#include "../headers/bankapp.h"
#include "ui_bankapp.h"
#include <QSqlQuery>
#include <QInputDialog>
#include <QMessageBox>
#include <QClipboard>
#include <QApplication>
#include <QStatusBar>
#include "../headers/database.h"

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
    
    if (dialog.exec() == QDialog::Accepted)
        on_comboAccounts_currentIndexChanged(ui->comboAccounts->currentIndex());
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