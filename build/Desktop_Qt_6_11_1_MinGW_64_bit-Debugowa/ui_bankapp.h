/********************************************************************************
** Form generated from reading UI file 'bankapp.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BANKAPP_H
#define UI_BANKAPP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BankApp
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *dashboardPage;
    QVBoxLayout *vbox2;
    QLabel *labelWelcome;
    QLabel *labelSelectAcc;
    QComboBox *comboAccounts;
    QLabel *labelBalance;
    QPushButton *btnDeposit;
    QPushButton *btnWithdraw;
    QPushButton *btnLogout;

    void setupUi(QMainWindow *BankApp)
    {
        if (BankApp->objectName().isEmpty())
            BankApp->setObjectName("BankApp");
        BankApp->resize(400, 350);
        centralwidget = new QWidget(BankApp);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        dashboardPage = new QWidget();
        dashboardPage->setObjectName("dashboardPage");
        vbox2 = new QVBoxLayout(dashboardPage);
        vbox2->setObjectName("vbox2");
        labelWelcome = new QLabel(dashboardPage);
        labelWelcome->setObjectName("labelWelcome");

        vbox2->addWidget(labelWelcome);

        labelSelectAcc = new QLabel(dashboardPage);
        labelSelectAcc->setObjectName("labelSelectAcc");

        vbox2->addWidget(labelSelectAcc);

        comboAccounts = new QComboBox(dashboardPage);
        comboAccounts->setObjectName("comboAccounts");

        vbox2->addWidget(comboAccounts);

        labelBalance = new QLabel(dashboardPage);
        labelBalance->setObjectName("labelBalance");

        vbox2->addWidget(labelBalance);

        btnDeposit = new QPushButton(dashboardPage);
        btnDeposit->setObjectName("btnDeposit");

        vbox2->addWidget(btnDeposit);

        btnWithdraw = new QPushButton(dashboardPage);
        btnWithdraw->setObjectName("btnWithdraw");

        vbox2->addWidget(btnWithdraw);

        btnLogout = new QPushButton(dashboardPage);
        btnLogout->setObjectName("btnLogout");

        vbox2->addWidget(btnLogout);

        stackedWidget->addWidget(dashboardPage);

        verticalLayout->addWidget(stackedWidget);

        BankApp->setCentralWidget(centralwidget);

        retranslateUi(BankApp);

        stackedWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(BankApp);
    } // setupUi

    void retranslateUi(QMainWindow *BankApp)
    {
        BankApp->setWindowTitle(QCoreApplication::translate("BankApp", "System Bankowy z Wieloma Kontami", nullptr));
        labelWelcome->setText(QCoreApplication::translate("BankApp", "Witaj!", nullptr));
        labelSelectAcc->setText(QCoreApplication::translate("BankApp", "Wybierz konto bankowe:", nullptr));
        labelBalance->setText(QCoreApplication::translate("BankApp", "Stan konta: 0.00 PLN", nullptr));
        btnDeposit->setText(QCoreApplication::translate("BankApp", "Wp\305\202ata", nullptr));
        btnWithdraw->setText(QCoreApplication::translate("BankApp", "Wyp\305\202ata", nullptr));
        btnLogout->setText(QCoreApplication::translate("BankApp", "Wyloguj", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BankApp: public Ui_BankApp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BANKAPP_H
