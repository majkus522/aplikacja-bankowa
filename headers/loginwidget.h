#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class LoginWidget;
}
QT_END_NAMESPACE

class LoginWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit LoginWidget(QWidget *parent = nullptr);
        ~LoginWidget();

    signals:
        void loginSuccessful(int userId, const QString &username, bool isAdmin);
        void goToRegisterRequested();

    private slots:
        void on_btnLogin_clicked();
        void on_btnGoToRegister_clicked();

    private:
        Ui::LoginWidget *ui;
};