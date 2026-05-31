/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *loginPage;
    QVBoxLayout *vbox1;
    QLabel *label;
    QLineEdit *editUser;
    QLineEdit *editPass;
    QPushButton *btnLogin;

    void setupUi(QMainWindow *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName("Login");
        Login->resize(400, 350);
        centralwidget = new QWidget(Login);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        loginPage = new QWidget();
        loginPage->setObjectName("loginPage");
        vbox1 = new QVBoxLayout(loginPage);
        vbox1->setObjectName("vbox1");
        label = new QLabel(loginPage);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vbox1->addWidget(label);

        editUser = new QLineEdit(loginPage);
        editUser->setObjectName("editUser");

        vbox1->addWidget(editUser);

        editPass = new QLineEdit(loginPage);
        editPass->setObjectName("editPass");
        editPass->setEchoMode(QLineEdit::EchoMode::Password);

        vbox1->addWidget(editPass);

        btnLogin = new QPushButton(loginPage);
        btnLogin->setObjectName("btnLogin");

        vbox1->addWidget(btnLogin);

        stackedWidget->addWidget(loginPage);

        verticalLayout->addWidget(stackedWidget);

        Login->setCentralWidget(centralwidget);

        retranslateUi(Login);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QMainWindow *Login)
    {
        Login->setWindowTitle(QCoreApplication::translate("Login", "System Bankowy z Wieloma Kontami", nullptr));
        label->setText(QCoreApplication::translate("Login", "LOGOWANIE", nullptr));
        editUser->setPlaceholderText(QCoreApplication::translate("Login", "Login", nullptr));
        editPass->setPlaceholderText(QCoreApplication::translate("Login", "Has\305\202o", nullptr));
        btnLogin->setText(QCoreApplication::translate("Login", "Zaloguj", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
