#include "../headers/bankapp.h"
#include "ui_bankapp.h"
#include <QSqlQuery>
#include <QInputDialog>
#include <QMessageBox>
#include <QClipboard>
#include <QApplication>
#include <QStatusBar>
#include "../headers/database.h"
#include <QSqlError>
#include <QDate>

BankApp::BankApp(QWidget *parent) : QMainWindow(parent), ui(new Ui::BankApp), loggedUserId(-1), currentAccountId(-1), currentBalance(0.0) 
{
    ui->setupUi(this);

    ui->btnOpenAdminPanel->hide();

    loginWidget = new LoginWidget(this);
    registerWidget = new RegisterWidget(this);

    ui->stackedWidget->addWidget(loginWidget);
    ui->stackedWidget->addWidget(registerWidget);

    connect(loginWidget, &LoginWidget::loginSuccessful, this, &BankApp::handleLoginSuccessful);
    connect(loginWidget, &LoginWidget::goToRegisterRequested, this, &BankApp::showRegisterPage);
    connect(registerWidget, &RegisterWidget::cancelRequested, this, &BankApp::showLoginPage);
    connect(registerWidget, &RegisterWidget::registrationSuccessful, this, &BankApp::showLoginPage);

    ui->stackedWidget->setCurrentWidget(loginWidget);
}

BankApp::~BankApp()
{
    delete ui;
}

void BankApp::handleLoginSuccessful(int userId, const QString &username, bool isAdmin)
{
    loggedUserId = userId;
    loggedUsername = username;
    isLoggedUserAdmin = isAdmin;

    ui->labelWelcome->setText("Zalogowano: <b>" + loggedUsername + "</b>");
    
    if (isLoggedUserAdmin)
        ui->btnOpenAdminPanel->show();
    else
        ui->btnOpenAdminPanel->hide();

    ui->comboAccounts->blockSignals(true);
    loadUserAccounts();
    ui->comboAccounts->blockSignals(false);

    if (ui->comboAccounts->count() > 0)
        on_comboAccounts_currentIndexChanged(0);
    ui->stackedWidget->setCurrentWidget(ui->dashboardPage);

    updateUserStatistics();
}

void BankApp::showRegisterPage()
{
    ui->stackedWidget->setCurrentWidget(registerWidget);
}

void BankApp::showLoginPage()
{
    ui->stackedWidget->setCurrentWidget(loginWidget);
}

void BankApp::loadUserAccounts()
{
    ui->comboAccounts->clear();
    QSqlQuery query;
    query.prepare("SELECT id, account_name, account_number FROM accounts WHERE user_id = :uid");
    query.bindValue(":uid", loggedUserId);

    if (query.exec())
    {
        while (query.next())
        {
            ui->comboAccounts->addItem(
                QString("%1 (%2)").arg(query.value(1).toString(), query.value(2).toString()), 
                query.value(0).toInt()
            );
        }
    }
}

void BankApp::on_comboAccounts_currentIndexChanged(int index)
{
    if (index < 0) return;
    currentAccountId = ui->comboAccounts->itemData(index).toInt();
    QSqlQuery query;
    query.prepare("SELECT balance FROM accounts WHERE id = :aid");
    query.bindValue(":aid", currentAccountId);
    if (query.exec() && query.next())
    {
        currentBalance = query.value(0).toDouble();
        updateBalanceDisplay();
    }

    updateUserStatistics();
}

void BankApp::updateBalanceDisplay()
{
    ui->labelBalance->setText(QString("Stan konta: <b style='color:green;'>%1 PLN</b>").arg(currentBalance, 0, 'f', 2));
}

void BankApp::on_btnLogout_clicked()
{
    Database::logActivity(loggedUserId, "LOGOUT", "Użytkownik wylogował się z systemu.");
    loggedUserId = -1; currentAccountId = -1;
    ui->comboAccounts->clear();
    showLoginPage();
}

void BankApp::on_btnOpenTransferDialog_clicked()
{
    if (currentAccountId == -1)
    {
        QMessageBox::warning(this, "Błąd", "Wybierz konto przed wykonaniem przelewu.");
        return;
    }

    TransferDialog dialog(currentAccountId, currentBalance, this);

    if (dialog.exec() == QDialog::Accepted) {
        on_comboAccounts_currentIndexChanged(ui->comboAccounts->currentIndex());
        updateUserStatistics();
    }
}

void BankApp::on_btnOpenHistoryDialog_clicked()
{
    if (currentAccountId == -1)
    {
        QMessageBox::warning(this, "Błąd", "Wybierz konto, aby zobaczyć jego historię.");
        return;
    }

    HistoryDialog dialog(currentAccountId, this);
    dialog.exec(); 
}

void BankApp::on_btnCopyAccountNumber_clicked()
{
    int currentIndex = ui->comboAccounts->currentIndex();
    if (currentIndex < 0)
    {
        QMessageBox::warning(this, "Błąd", "Nie wybrano żadnego konta do skopiowania.");
        return;
    }

    QString fullText = ui->comboAccounts->currentText();

    int openBracket = fullText.lastIndexOf('(');
    int closeBracket = fullText.lastIndexOf(')');

    if (openBracket != -1 && closeBracket != -1 && closeBracket > openBracket)
    {
        QString accountNumber = fullText.mid(openBracket + 1, closeBracket - openBracket - 1);
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(accountNumber);
        this->statusBar()->showMessage("Numer konta został skopiowany do schowka!", 3000);
    }
    else
        QMessageBox::critical(this, "Błąd", "Nie udało się poprawnie sparsować numeru konta.");
}

void BankApp::on_btnOpenAdminPanel_clicked()
{
    if (!isLoggedUserAdmin) return; 

    AdminWidget adminDialog(loggedUserId, this);
    adminDialog.refreshData();
    
    adminDialog.exec(); 
    
    on_comboAccounts_currentIndexChanged(ui->comboAccounts->currentIndex());
}

void BankApp::updateUserStatistics() {
    if (loggedUserId == -1) return;

    int currentMonth = QDate::currentDate().month();
    int currentYear = QDate::currentDate().year();

    QSqlQuery querySavings;
    querySavings.prepare("SELECT COALESCE(SUM(balance), 0) FROM accounts WHERE user_id = :uid");
    querySavings.bindValue(":uid", loggedUserId);
    if (querySavings.exec() && querySavings.next()) {
        double totalSavings = querySavings.value(0).toDouble();
        ui->labelTotalSavings->setText(QString("Łączne oszczędności: <b>%1 PLN</b>").arg(QString::number(totalSavings, 'f', 2)));
    }

    QSqlQuery queryExpenses;
    queryExpenses.prepare(
        "SELECT COALESCE(SUM(t.amount), 0) FROM transactions t "
        "JOIN accounts a ON t.sender_account_id = a.id "
        "WHERE a.id = :uid "
        "AND EXTRACT(MONTH FROM t.created_at) = :month "
        "AND EXTRACT(YEAR FROM t.created_at) = :year"
    );
    queryExpenses.bindValue(":uid", currentAccountId);
    queryExpenses.bindValue(":month", currentMonth);
    queryExpenses.bindValue(":year", currentYear);
    if (queryExpenses.exec() && queryExpenses.next()) {
        double monthlyExpenses = queryExpenses.value(0).toDouble();
        ui->labelMonthlyExpenses->setText(QString("Wydatki w tym miesiącu: <font color='red'><b>-%1 PLN</b></font>").arg(QString::number(monthlyExpenses, 'f', 2)));
    }

    QSqlQuery queryIncomes;
    queryIncomes.prepare(
        "SELECT COALESCE(SUM(t.amount), 0) FROM transactions t "
        "JOIN accounts a ON t.receiver_account_id = a.id "
        "WHERE a.id = :uid "
        "AND EXTRACT(MONTH FROM t.created_at) = :month "
        "AND EXTRACT(YEAR FROM t.created_at) = :year"
    );
    queryIncomes.bindValue(":uid", currentAccountId);
    queryIncomes.bindValue(":month", currentMonth);
    queryIncomes.bindValue(":year", currentYear);
    if (queryIncomes.exec() && queryIncomes.next()) {
        double monthlyIncomes = queryIncomes.value(0).toDouble();
        ui->labelMonthlyIncomes->setText(QString("Wpływy w tym miesiącu: <font color='green'><b>+%1 PLN</b></font>").arg(QString::number(monthlyIncomes, 'f', 2)));
    }

    QSqlQuery queryMaxExpense;
    queryMaxExpense.prepare(
        "SELECT COALESCE(MAX(t.amount), 0), t.title FROM transactions t "
        "JOIN accounts a ON t.sender_account_id = a.id "
        "WHERE a.id = :uid "
        "GROUP BY t.title, t.amount "
        "ORDER BY t.amount DESC LIMIT 1"
    );
    queryMaxExpense.bindValue(":uid", currentAccountId);
    if (queryMaxExpense.exec() && queryMaxExpense.next()) {
        double maxExpense = queryMaxExpense.value(0).toDouble();
        QString title = queryMaxExpense.value(1).toString();
        if (maxExpense > 0) {
            ui->labelBiggestExpense->setText(QString("Największy wydatek: <b>%1 PLN</b> (<i>%2</i>)").arg(QString::number(maxExpense, 'f', 2)).arg(title));
        } else {
            ui->labelBiggestExpense->setText("Największy wydatek: <b>0.00 PLN</b>");
        }
    }

    QSqlQuery queryMaxIncome;
    queryMaxIncome.prepare(
        "SELECT COALESCE(MAX(t.amount), 0), t.title FROM transactions t "
        "JOIN accounts a ON t.sender_account_id = a.id "
        "WHERE a.id = :uid "
        "GROUP BY t.title, t.amount "
        "ORDER BY t.amount ASC LIMIT 1"
    );
    queryMaxIncome.bindValue(":uid", currentAccountId);
    if (queryMaxIncome.exec() && queryMaxIncome.next()) {
        double maxIncome = queryMaxIncome.value(0).toDouble();
        QString title = queryMaxIncome.value(1).toString();
        if (maxIncome > 0) {
            ui->labelBiggestIncome->setText(QString("Największy przychód: <b>%1 PLN</b> (<i>%2</i>)").arg(QString::number(maxIncome, 'f', 2)).arg(title));
        } else {
            ui->labelBiggestIncome->setText("Największy przychód: <b>0.00 PLN</b>");
        }
    }
}