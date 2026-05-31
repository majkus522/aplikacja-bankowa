#include "loginwidget.h"
#include "ui_loginwidget.h"
#include <QSqlQuery>
#include <QMessageBox>

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent), ui(new Ui::LoginWidget) {
    ui->setupUi(this);
}

LoginWidget::~LoginWidget() {
    delete ui;
}

void LoginWidget::on_btnLogin_clicked() {
    QString user = ui->editUser->text();
    QString pass = ui->editPass->text();

    QSqlQuery query;
    // DODAJEMY is_blocked DO ZAPYTANIA
    query.prepare("SELECT id, is_admin, is_blocked FROM users WHERE username = :u AND password = :p");
    query.bindValue(":u", user);
    query.bindValue(":p", pass);

    if (query.exec() && query.next()) {
        bool isBlocked = query.value(2).toBool(); // <-- Pobranie flagi blokady

        // SPRAWDZENIE BLOKADY
        if (isBlocked) {
            QMessageBox::critical(this, "Dostęp zabroniony", 
                                  "Twoje konto zostało zablokowane przez administratora. "
                                  "Skontaktuj się z infolinią banku.");
            return; // Przerywamy funkcję, użytkownik nie zostanie zalogowany
        }

        int userId = query.value(0).toInt();
        bool isAdmin = query.value(1).toBool();
        
        ui->editUser->clear();
        ui->editPass->clear();
        
        emit loginSuccessful(userId, user, isAdmin);
    } else {
        QMessageBox::warning(this, "Błąd", "Błędne dane logowania");
    }
}

void LoginWidget::on_btnGoToRegister_clicked() {
    emit goToRegisterRequested();
}