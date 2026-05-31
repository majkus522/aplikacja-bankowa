/********************************************************************************
** Form generated from reading UI file 'register.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTER_H
#define UI_REGISTER_H

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
    QWidget *registerPage;
    QVBoxLayout *vbox3;
    QLabel *labelReg;
    QLineEdit *editRegUser;
    QLineEdit *editRegPass;
    QLineEdit *editRegPassConfirm;
    QPushButton *btnRegisterSubmit;
    QPushButton *btnCancelRegister;

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
        registerPage = new QWidget();
        registerPage->setObjectName("registerPage");
        vbox3 = new QVBoxLayout(registerPage);
        vbox3->setObjectName("vbox3");
        labelReg = new QLabel(registerPage);
        labelReg->setObjectName("labelReg");
        labelReg->setAlignment(Qt::AlignCenter);

        vbox3->addWidget(labelReg);

        editRegUser = new QLineEdit(registerPage);
        editRegUser->setObjectName("editRegUser");

        vbox3->addWidget(editRegUser);

        editRegPass = new QLineEdit(registerPage);
        editRegPass->setObjectName("editRegPass");
        editRegPass->setEchoMode(QLineEdit::Password);

        vbox3->addWidget(editRegPass);

        editRegPassConfirm = new QLineEdit(registerPage);
        editRegPassConfirm->setObjectName("editRegPassConfirm");
        editRegPassConfirm->setEchoMode(QLineEdit::Password);

        vbox3->addWidget(editRegPassConfirm);

        btnRegisterSubmit = new QPushButton(registerPage);
        btnRegisterSubmit->setObjectName("btnRegisterSubmit");

        vbox3->addWidget(btnRegisterSubmit);

        btnCancelRegister = new QPushButton(registerPage);
        btnCancelRegister->setObjectName("btnCancelRegister");
        btnCancelRegister->setFlat(true);

        vbox3->addWidget(btnCancelRegister);

        stackedWidget->addWidget(registerPage);

        verticalLayout->addWidget(stackedWidget);

        Login->setCentralWidget(centralwidget);

        retranslateUi(Login);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QMainWindow *Login)
    {
        Login->setWindowTitle(QCoreApplication::translate("Login", "System Bankowy z Wieloma Kontami", nullptr));
        labelReg->setText(QCoreApplication::translate("Login", "REJESTRACJA NOWEGO U\305\273YTKOWNIKA", nullptr));
        editRegUser->setPlaceholderText(QCoreApplication::translate("Login", "Nowy login", nullptr));
        editRegPass->setPlaceholderText(QCoreApplication::translate("Login", "Has\305\202o", nullptr));
        editRegPassConfirm->setPlaceholderText(QCoreApplication::translate("Login", "Powt\303\263rz has\305\202o", nullptr));
        btnRegisterSubmit->setText(QCoreApplication::translate("Login", "Utw\303\263rz konto", nullptr));
        btnCancelRegister->setText(QCoreApplication::translate("Login", "Powr\303\263t do logowania", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTER_H
