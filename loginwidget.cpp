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
    query.prepare("SELECT id FROM users WHERE username = :u AND password = :p");
    query.bindValue(":u", user);
    query.bindValue(":p", pass);

    if (query.exec() && query.next()) {
        int userId = query.value(0).toInt();
        ui->editUser->clear();
        ui->editPass->clear();
        emit loginSuccessful(userId, user); // Emitujemy sygnał do głównego okna
    } else {
        QMessageBox::warning(this, "Błąd", "Błędne dane logowania");
    }
}

void LoginWidget::on_btnGoToRegister_clicked() {
    emit goToRegisterRequested();
}