#include "bankapp.h"
#include "ui_bankapp.h"
#include <QSqlQuery>
#include <QInputDialog>
#include <QMessageBox>

BankApp::BankApp(QWidget *parent) : QMainWindow(parent), ui(new Ui::BankApp), loggedUserId(-1), currentAccountId(-1), currentBalance(0.0) 
{
    ui->setupUi(this);

    loginWidget = new Login(this);
    registerWidget = new Register(this);

    // 2. Dodajemy je dynamicznie do QStackedWidget i zapamiętujemy ich indeksy/wskaźniki
    // Przekazujemy 'ui->stackedWidget' jako rodzica
    ui->stackedWidget->addWidget(loginWidget);    // Zostanie dodany na pozycję (indeks) np. 1
    ui->stackedWidget->addWidget(registerWidget); // Zostanie dodany na pozycję np. 2

    // 3. Łączymy sygnały (Signals) dokładnie tak samo jak wcześniej
    connect(loginWidget, &Login::loginSuccessful, this, &BankApp::handleLoginSuccessful);
    connect(loginWidget, &Login::goToRegisterRequested, this, [=](){
        ui->stackedWidget->setCurrentWidget(registerWidget);
    });
    
    connect(registerWidget, &Register::cancelRequested, this, [=](){
        ui->stackedWidget->setCurrentWidget(loginWidget);
    });
    connect(registerWidget, &Register::registrationSuccessful, this, [=](){
        ui->stackedWidget->setCurrentWidget(loginWidget);
    });

    // 4. Ustawiamy ekran startowy na widget logowania
    ui->stackedWidget->setCurrentWidget(loginWidget);
}

BankApp::~BankApp() {
    delete ui;
}

void BankApp::handleLoginSuccessful(int userId, const QString &username) {
    loggedUserId = userId;
    loggedUsername = username;

    ui->labelWelcome->setText("Zalogowano: <b>" + loggedUsername + "</b>");
    
    ui->comboAccounts->blockSignals(true);
    loadUserAccounts();
    ui->comboAccounts->blockSignals(false);

    if (ui->comboAccounts->count() > 0) {
        on_comboAccounts_currentIndexChanged(0);
    }

    ui->stackedWidget->setCurrentWidget(ui->dashboardPage); // Przełącz na pulpit
}

void BankApp::showRegisterPage() {
    ui->stackedWidget->setCurrentWidget(ui->registerPage);
}

void BankApp::showLoginPage() {
    ui->stackedWidget->setCurrentWidget(ui->loginPage);
}

// === LOGIKA PULPITU (DASHBOARD) ===

void BankApp::loadUserAccounts() {
    ui->comboAccounts->clear();
    QSqlQuery query;
    query.prepare("SELECT id, account_name, account_number FROM accounts WHERE user_id = :uid");
    query.bindValue(":uid", loggedUserId);

    if (query.exec()) {
        while (query.next()) {
            ui->comboAccounts->addItem(
                QString("%1 (%2)").arg(query.value(1).toString(), query.value(2).toString()), 
                query.value(0).toInt()
            );
        }
    }
}

void BankApp::on_comboAccounts_currentIndexChanged(int index) {
    if (index < 0) return;
    currentAccountId = ui->comboAccounts->itemData(index).toInt();
    QSqlQuery query;
    query.prepare("SELECT balance FROM accounts WHERE id = :aid");
    query.bindValue(":aid", currentAccountId);
    if (query.exec() && query.next()) {
        currentBalance = query.value(0).toDouble();
        updateBalanceDisplay();
    }
}

void BankApp::updateBalanceDisplay() {
    ui->labelBalance->setText(QString("Stan konta: <b style='color:green;'>%1 PLN</b>").arg(currentBalance, 0, 'f', 2));
}

void BankApp::on_btnDeposit_clicked() {
    if (currentAccountId == -1) return;
    bool ok;
    double amount = QInputDialog::getDouble(this, "Wpłata", "Kwota:", 0, 0, 100000, 2, &ok);
    if (ok && amount > 0) {
        QSqlQuery q;
        q.prepare("UPDATE accounts SET balance = balance + :a WHERE id = :aid");
        q.bindValue(":a", amount); q.bindValue(":aid", currentAccountId);
        if (q.exec()) { currentBalance += amount; updateBalanceDisplay(); }
    }
}

void BankApp::on_btnWithdraw_clicked() {
    if (currentAccountId == -1) return;
    bool ok;
    double amount = QInputDialog::getDouble(this, "Wypłata", "Kwota:", 0, 0, currentBalance, 2, &ok);
    if (ok && amount > 0 && amount <= currentBalance) {
        QSqlQuery q;
        q.prepare("UPDATE accounts SET balance = balance - :a WHERE id = :aid");
        q.bindValue(":a", amount); q.bindValue(":aid", currentAccountId);
        if (q.exec()) { currentBalance -= amount; updateBalanceDisplay(); }
    }
}

void BankApp::on_btnLogout_clicked() {
    loggedUserId = -1; currentAccountId = -1;
    ui->comboAccounts->clear();
    showLoginPage();
}