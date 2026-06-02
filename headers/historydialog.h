#pragma once

#include <QDialog>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class HistoryDialog;
}
QT_END_NAMESPACE

class HistoryDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit HistoryDialog(int accountId, QWidget *parent = nullptr);
        ~HistoryDialog();

    private slots:
        void on_btnClose_clicked();

    private:
        Ui::HistoryDialog *ui;
        QSqlQueryModel *transactionsModel;
        QSortFilterProxyModel *proxyModel;
        int m_accountId;
        void loadTransactions();
};