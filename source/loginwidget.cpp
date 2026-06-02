#include "../headers/loginwidget.h"
#include "ui_loginwidget.h"
#include <QSqlQuery>
#include <QMessageBox>
#include "../headers/database.h"

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent), ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::on_btnLogin_clicked()
{
    QString user = ui->editUser->text();
    QString pass = ui->editPass->text();

    QSqlQuery query;
    query.prepare("SELECT id, is_admin, is_blocked FROM users WHERE username = :u AND password = :p");
    query.bindValue(":u", user);
    query.bindValue(":p", pass);

    if (query.exec() && query.next())
    {
        bool isBlocked = query.value(2).toBool();
        int userId = query.value(0).toInt();
        if (isBlocked) {
            Database::logActivity(userId, "LOGIN_BLOCKED", "Odmowa dostępu: konto jest zablokowane.");
            QMessageBox::critical(this, "Dostęp zabroniony", 
                                  "Twoje konto zostało zablokowane przez administratora. "
                                  "Skontaktuj się z infolinią banku.");
            return;
        }
        bool isAdmin = query.value(1).toBool();
        
        ui->editUser->clear();
        ui->editPass->clear();

        Database::logActivity(userId, "LOGIN", QString("Użytkownik zalogował się do systemu (Admin: %1)").arg(isAdmin ? "TAK" : "NIE"));
        
        emit loginSuccessful(userId, user, isAdmin);
    }
    else
    {
        Database::logActivity(-1, "LOGIN_FAILED", QString("Nieudana próba logowania na login: '%1'").arg(user));
        QMessageBox::warning(this, "Błąd", "Błędne dane logowania");
    }
}

void LoginWidget::on_btnGoToRegister_clicked()
{
    emit goToRegisterRequested();
}