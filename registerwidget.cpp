#include "registerwidget.h"
#include "ui_registerwidget.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QRandomGenerator>

RegisterWidget::RegisterWidget(QWidget *parent) : QWidget(parent), ui(new Ui::RegisterWidget) {
    ui->setupUi(this);
}

RegisterWidget::~RegisterWidget() {
    delete ui;
}

void RegisterWidget::on_btnRegisterSubmit_clicked() {
    QString user = ui->editRegUser->text().trimmed();
    QString pass = ui->editRegPass->text();
    QString passConfirm = ui->editRegPassConfirm->text();

    if (user.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Pola nie mogą być puste!");
        return;
    }
    if (pass != passConfirm) {
        QMessageBox::warning(this, "Błąd", "Hasła nie są identyczne!");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.transaction()) return;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT id FROM users WHERE username = :u");
    checkQuery.bindValue(":u", user);
    if (checkQuery.exec() && checkQuery.next()) {
        QMessageBox::warning(this, "Błąd", "Login jest zajęty!");
        db.rollback();
        return;
    }

    QSqlQuery insUser;
    insUser.prepare("INSERT INTO users (username, password) VALUES (:u, :p) RETURNING id");
    insUser.bindValue(":u", user);
    insUser.bindValue(":p", pass);

    if (!insUser.exec() || !insUser.next()) {
        db.rollback();
        return;
    }

    int newUserId = insUser.value(0).toInt();

    QSqlQuery insAcc;
    insAcc.prepare("INSERT INTO accounts (user_id, account_number, account_name, balance) VALUES (:uid, :anum, 'Konto ROR', 0.0)");
    insAcc.bindValue(":uid", newUserId);
    insAcc.bindValue(":anum", generateRandomAccountNumber());

    if (insAcc.exec() && db.commit()) {
        QMessageBox::information(this, "Sukces", "Konto utworzone!");
        emit registrationSuccessful();
    } else {
        db.rollback();
    }
}

void RegisterWidget::on_btnCancelRegister_clicked() {
    emit cancelRequested();
}

QString RegisterWidget::generateRandomAccountNumber() {
    QString number = "";
    for(int i = 0; i < 26; ++i) {
        number.append(QString::number(QRandomGenerator::global()->bounded(0, 10)));
    }
    return number;
}