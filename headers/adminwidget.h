#ifndef ADMINWIDGET_H
#define ADMINWIDGET_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui { class AdminWidget; }

class AdminWidget : public QDialog {
    Q_OBJECT

public:
    explicit AdminWidget(int adminId, QWidget *parent = nullptr);
    ~AdminWidget();
    void refreshData(); // Metoda do odświeżania listy użytkowników po wejściu do panelu

signals:
    void logoutRequested();

private slots:
    void on_btnLogOutAdmin_clicked();
    void on_btnToggleAdmin_clicked();
    void on_btnToggleBlock_clicked();

private:
    Ui::AdminWidget *ui;
    QSqlQueryModel *usersModel;
    QSqlQueryModel *logModel;
    int m_adminId;
};

#endif // ADMINWIDGET_H