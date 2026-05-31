#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>

namespace Ui { class Register; }

class Register : public QWidget {
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

signals:
    void cancelRequested();
    void registrationSuccessful();

private slots:
    void on_btnRegisterSubmit_clicked();
    void on_btnCancelRegister_clicked();

private:
    Ui::Register *ui;
    QString generateRandomAccountNumber();
};

#endif // REGISTER_H