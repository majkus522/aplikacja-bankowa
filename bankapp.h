#ifndef BANKAPP_H
#define BANKAPP_H

#include <QMainWindow>
#include "loginwidget.h"    // Pamiętaj o dodaniu tych include'ów
#include "registerwidget.h"
#include "transferdialog.h"
#include "historydialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class BankApp; }
QT_END_NAMESPACE

class BankApp : public QMainWindow {
    Q_OBJECT

public:
    BankApp(QWidget *parent = nullptr);
    ~BankApp();

private slots:
    // Slot obsłużony z sygnału LoginWidget
    void handleLoginSuccessful(int userId, const QString &username);
    
    // Sloty nawigacyjne
    void showRegisterPage();
    void showLoginPage();

    // Sloty obsługi konta (Dashboard)
    void on_btnLogout_clicked();
    void on_comboAccounts_currentIndexChanged(int index);
    void on_btnOpenTransferDialog_clicked();
    void on_btnOpenHistoryDialog_clicked();

private:
    Ui::BankApp *ui;
    int loggedUserId;
    QString loggedUsername;
    int currentAccountId;
    double currentBalance;

    LoginWidget *loginWidget;
    RegisterWidget *registerWidget;

    void loadUserAccounts();
    void updateBalanceDisplay();
};

#endif // BANKAPP_H