#include "bankapp.h"
#include "ui_bankapp.h"
#include <QSqlQuery>
#include <QInputDialog>
#include <QMessageBox>
#include <QClipboard>
#include <QApplication>
#include <QStatusBar>

BankApp::BankApp(QWidget *parent) 
    : QMainWindow(parent), ui(new Ui::BankApp), loggedUserId(-1), currentAccountId(-1), currentBalance(0.0) 
{
    ui->setupUi(this);

    loginWidget = new LoginWidget(this);
    registerWidget = new RegisterWidget(this);

    // 2. Dodajemy je dynamicznie do QStackedWidget i zapamiętujemy ich indeksy/wskaźniki
    // Przekazujemy 'ui->stackedWidget' jako rodzica
    ui->stackedWidget->addWidget(loginWidget);    // Zostanie dodany na pozycję (indeks) np. 1
    ui->stackedWidget->addWidget(registerWidget); // Zostanie dodany na pozycję np. 2

    // 3. Łączymy sygnały (Signals) dokładnie tak samo jak wcześniej
    connect(loginWidget, &LoginWidget::loginSuccessful, this, &BankApp::handleLoginSuccessful);
    connect(loginWidget, &LoginWidget::goToRegisterRequested, this, [=](){
        ui->stackedWidget->setCurrentWidget(registerWidget);
    });
    
    connect(registerWidget, &RegisterWidget::cancelRequested, this, [=](){
        ui->stackedWidget->setCurrentWidget(loginWidget);
    });
    connect(registerWidget, &RegisterWidget::registrationSuccessful, this, [=](){
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
    ui->stackedWidget->setCurrentWidget(registerWidget);
}

void BankApp::showLoginPage() {
    ui->stackedWidget->setCurrentWidget(loginWidget);
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

void BankApp::on_btnLogout_clicked() {
    loggedUserId = -1; currentAccountId = -1;
    ui->comboAccounts->clear();
    showLoginPage();
}

void BankApp::on_btnOpenTransferDialog_clicked() {
    if (currentAccountId == -1) {
        QMessageBox::warning(this, "Błąd", "Wybierz konto przed wykonaniem przelewu.");
        return;
    }

    // Tworzymy okno dialogowe, przekazując ID konta i aktualne saldo
    TransferDialog dialog(currentAccountId, currentBalance, this);
    
    // Uruchamiamy okno w trybie modalnym
    if (dialog.exec() == QDialog::Accepted) {
        // Jeśli przelew się udał (funkcja accept() w dialogu), 
        // odświeżamy saldo, wymuszając przeładowanie danych z bazy
        on_comboAccounts_currentIndexChanged(ui->comboAccounts->currentIndex());
    }
}

void BankApp::on_btnOpenHistoryDialog_clicked() {
    if (currentAccountId == -1) {
        QMessageBox::warning(this, "Błąd", "Wybierz konto, aby zobaczyć jego historię.");
        return;
    }

    // Tworzymy i uruchamiamy modalne okno historii
    HistoryDialog dialog(currentAccountId, this);
    dialog.exec(); 
}

void BankApp::on_btnCopyAccountNumber_clicked() {
    // Sprawdzamy, czy użytkownik ma wybrane jakiekolwiek konto
    int currentIndex = ui->comboAccounts->currentIndex();
    if (currentIndex < 0) {
        QMessageBox::warning(this, "Błąd", "Nie wybrano żadnego konta do skopiowania.");
        return;
    }

    // Pobieramy pełny tekst z ComboBoxa, np. "Konto Osobiste ROR (12345678901234567890123456)"
    QString fullText = ui->comboAccounts->currentText();

    // Wyciągamy sam numer konta, który znajduje się wewnątrz nawiasów
    int openBracket = fullText.lastIndexOf('(');
    int closeBracket = fullText.lastIndexOf(')');

    if (openBracket != -1 && closeBracket != -1 && closeBracket > openBracket) {
        // Wycinamy zawartość między nawiasami (dokładnie 26 cyfr)
        QString accountNumber = fullText.mid(openBracket + 1, closeBracket - openBracket - 1);

        // Kopiowanie do schowka systemowego
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(accountNumber);

        // Opcjonalne powiadomienie użytkownika w pasku stanu (StatusBar) okna głównego,
        // aby nie męczyć go wyskakującymi okienkami QMessageBox
        this->statusBar()->showMessage("Numer konta został skopiowany do schowka!", 3000); // Komunikat zniknie po 3 sekundach
    } else {
        QMessageBox::critical(this, "Błąd", "Nie udało się poprawnie sparsować numeru konta.");
    }
}