#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include <QWidget>

namespace Ui { class RegisterWidget; }

class RegisterWidget : public QWidget {
    Q_OBJECT

public:
    explicit RegisterWidget(QWidget *parent = nullptr);
    ~RegisterWidget();

signals:
    void cancelRequested();
    void registrationSuccessful();

private slots:
    void on_btnRegisterSubmit_clicked();
    void on_btnCancelRegister_clicked();

private:
    Ui::RegisterWidget *ui;
    QString generateRandomAccountNumber();
};

#endif // REGISTERWIDGET_H