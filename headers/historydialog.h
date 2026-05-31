#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel> // <-- DODAJ TEN INCLUDE

namespace Ui { class HistoryDialog; }

class HistoryDialog : public QDialog {
    Q_OBJECT

public:
    explicit HistoryDialog(int accountId, QWidget *parent = nullptr);
    ~HistoryDialog();

private slots:
    void on_btnClose_clicked();

private:
    Ui::HistoryDialog *ui;
    QSqlQueryModel *transactionsModel;
    QSortFilterProxyModel *proxyModel; // <-- NOWY WSKAŹNIK
    int m_accountId;

    void loadTransactions();
};

#endif // HISTORYDIALOG_H