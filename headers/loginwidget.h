#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

namespace Ui { class LoginWidget; }

class LoginWidget : public QWidget {
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

#endif // LOGINWIDGET_H