#pragma once

#include <QDialog>
#include <QSqlQueryModel>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class AdminWidget;
}
QT_END_NAMESPACE

class AdminWidget : public QDialog
{
    Q_OBJECT

    public:
        explicit AdminWidget(int adminId, QWidget *parent = nullptr);
        ~AdminWidget();
        void refreshData();

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