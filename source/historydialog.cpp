#include "../headers/historydialog.h"
#include "ui_historydialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QHeaderView>

HistoryDialog::HistoryDialog(int accountId, QWidget *parent) : QDialog(parent), ui(new Ui::HistoryDialog), m_accountId(accountId) 
{
    ui->setupUi(this);
    setWindowTitle("Historia Operacji");

    transactionsModel = new QSqlQueryModel(this);
    
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(transactionsModel);
    
    proxyModel->setSortRole(Qt::EditRole); 

    ui->viewTransactions->setModel(proxyModel);
    ui->viewTransactions->setSortingEnabled(true);
    ui->viewTransactions->sortByColumn(0, Qt::DescendingOrder);
    ui->viewTransactions->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->viewTransactions->setSelectionBehavior(QAbstractItemView::SelectRows);

    loadTransactions();
}

HistoryDialog::~HistoryDialog()
{
    delete ui;
}

void HistoryDialog::loadTransactions()
{
    QSqlQuery query;
    query.prepare(
        "SELECT "
        "  created_at AS \"Data\", "
        "  title AS \"Tytuł\", "
        "  CASE "
        "    WHEN sender_account_id = :aid THEN -amount "
        "    ELSE amount "
        "  END AS \"Kwota (PLN)\" "
        "FROM transactions "
        "WHERE sender_account_id = :aid OR receiver_account_id = :aid"
    );
    query.bindValue(":aid", m_accountId);

    if (query.exec())
        transactionsModel->setQuery(std::move(query));
}

void HistoryDialog::on_btnClose_clicked()
{
    accept();
}