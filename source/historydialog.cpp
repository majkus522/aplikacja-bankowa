#include "../headers/historydialog.h"
#include "ui_historydialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QHeaderView>

HistoryDialog::HistoryDialog(int accountId, QWidget *parent) 
    : QDialog(parent), ui(new Ui::HistoryDialog), m_accountId(accountId) 
{
    ui->setupUi(this);
    setWindowTitle("Historia Operacji");

    // 1. Inicjalizacja bazowego modelu SQL
    transactionsModel = new QSqlQueryModel(this);
    
    // 2. Inicjalizacja modelu pośredniczącego (Proxy) do sortowania
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(transactionsModel);
    
    // Ważne: Jeśli w bazie kwoty są tekstowe/numeryczne, proxy model 
    // automatycznie dopasuje sortowanie (alfabetyczne lub numeryczne)
    proxyModel->setSortRole(Qt::EditRole); 

    // 3. Przypisanie modelu Proxy do widoku tabeli
    ui->viewTransactions->setModel(proxyModel);
    
    // 4. Włączenie sortowania w interfejsie użytkownika
    ui->viewTransactions->setSortingEnabled(true);
    
    // Domyślne sortowanie na starcie po pierwszej kolumnie (Data) malejąco (Descending)
    ui->viewTransactions->sortByColumn(0, Qt::DescendingOrder);

    // Formatowanie wyglądu tabeli
    ui->viewTransactions->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->viewTransactions->setSelectionBehavior(QAbstractItemView::SelectRows);

    loadTransactions();
}

HistoryDialog::~HistoryDialog() {
    delete ui;
}

void HistoryDialog::loadTransactions() {
    QSqlQuery query;
    // Ważne: usuwamy z zapytania SQL "ORDER BY", ponieważ 
    // teraz to Qt (Proxy Model) w locie zarządza kolejnością wyświetlania.
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

    if (query.exec()) {
        transactionsModel->setQuery(std::move(query));
    }
}

void HistoryDialog::on_btnClose_clicked() {
    accept();
}