#include "../headers/adminwidget.h"
#include "ui_adminwidget.h"
#include <QSqlQuery>
#include <QHeaderView>
#include <QMessageBox>
#include "../headers/database.h"

AdminWidget::AdminWidget(int adminId, QWidget *parent) : QDialog(parent), ui(new Ui::AdminWidget), m_adminId(adminId) {
    ui->setupUi(this);
    
    usersModel = new QSqlQueryModel(this);
    ui->viewAllUsers->setModel(usersModel);
    ui->viewAllUsers->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    logModel = new QSqlQueryModel(this);
    ui->viewLogs->setModel(logModel);
    ui->viewLogs->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // domyślne
    ui->viewLogs->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->viewLogs->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

AdminWidget::~AdminWidget() {
    delete ui;
}

void AdminWidget::refreshData() {
    QSqlQuery query;
    query.prepare(
        "SELECT u.id AS \"ID\", u.username AS \"Użytkownik\", "
        "CASE WHEN u.is_admin THEN 'TAK' ELSE 'NIE' END AS \"Administrator\", "
        "CASE WHEN u.is_blocked THEN 'ZABLOKOWANY' ELSE 'AKTYWNY' END AS \"Status\", " // <-- NOWA KOLUMNA
        "COALESCE(SUM(a.balance), 0) AS \"Łączne środki (PLN)\", "
        "COUNT(a.id) AS \"Liczba kont\" "
        "FROM users u "
        "LEFT JOIN accounts a ON u.id = a.user_id "
        "GROUP BY u.id, u.username, u.is_admin, u.is_blocked " // <-- Dodano u.is_blocked
        "ORDER BY u.id"
    );
    
    if (query.exec()) {
        usersModel->setQuery(std::move(query));
    }

    QSqlQuery logQuery;
    logQuery.prepare(
        "SELECT l.created_at AS \"Data\", u.username AS \"Użytkownik\", "
        "l.action_type AS \"Akcja\", l.description AS \"Opis zdarzenia\" "
        "FROM activity_logs l "
        "LEFT JOIN users u ON l.user_id = u.id "
        "ORDER BY l.created_at DESC"
    );
    if (logQuery.exec()) {
        logModel->setQuery(std::move(logQuery));
    }
}

void AdminWidget::on_btnLogOutAdmin_clicked() {
    accept();
}

void AdminWidget::on_btnToggleAdmin_clicked() {
    // 1. Pobieramy model zaznaczenia tabeli
    QItemSelectionModel *selection = ui->viewAllUsers->selectionModel();
    
    if (!selection->hasSelection()) {
        QMessageBox::warning(this, "Błąd", "Wybierz użytkownika z listy, aby zmienić jego uprawnienia.");
        return;
    }

    // 2. Pobieramy indeks zaznaczonego wiersza (bierzemy pierwszą kolumnę - ID)
    int currentRow = selection->currentIndex().row();
    
    // index(wiersz, kolumna) -> kolumna 0 to "ID" w naszym zapytaniu SQL
    QModelIndex idIndex = usersModel->index(currentRow, 0); 
    QModelIndex adminIndex = usersModel->index(currentRow, 2); // kolumna 2 to "Administrator" (TAK/NIE)

    int userId = usersModel->data(idIndex).toInt();
    QString isAdminStr = usersModel->data(adminIndex).toString();

    // Zabezpieczenie: admin nie powinien móc odebrać praw samemu sobie przez przypadek
    // (Możesz opcjonalnie przekazać ID aktualnego admina z BankApp, aby to zaimplementować idealnie)

    // 3. Określamy nową wartość na podstawie starej
    bool newAdminStatus = (isAdminStr == "NIE"); 

    // 4. Aktualizacja w bazie danych PostgreSQL
    QSqlQuery query;
    query.prepare("UPDATE users SET is_admin = :status WHERE id = :uid");
    query.bindValue(":status", newAdminStatus);
    query.bindValue(":uid", userId);

    if (query.exec()) {
        QMessageBox::information(this, "Sukces", "Uprawnienia użytkownika zostały pomyślnie zmienione.");;
        Database::logActivity(m_adminId, "ADMIN_ACTION", QString("Administrator zmienił status użytkownika o ID %1 na: %2").arg(userId).arg(newAdminStatus ? "ADMIN" : "UŻYTKOWNIK"));
        refreshData(); // Odświeżamy tabelę, aby pokazać nowe dane
    } else {
        QMessageBox::critical(this, "Błąd", "Nie udało się zaktualizować uprawnień w bazie danych.");
    }
}

void AdminWidget::on_btnToggleBlock_clicked() {
    QItemSelectionModel *selection = ui->viewAllUsers->selectionModel();
    if (!selection->hasSelection()) {
        QMessageBox::warning(this, "Błąd", "Wybierz użytkownika z listy.");
        return;
    }

    int currentRow = selection->currentIndex().row();
    int userId = usersModel->data(usersModel->index(currentRow, 0)).toInt();
    QString statusStr = usersModel->data(usersModel->index(currentRow, 3)).toString(); // Kolumna 3 to "Status"

    // Odwracamy stan blokady
    bool newBlockStatus = (statusStr == "AKTYWNY");

    QSqlQuery query;
    query.prepare("UPDATE users SET is_blocked = :status WHERE id = :uid");
    query.bindValue(":status", newBlockStatus);
    query.bindValue(":uid", userId);

    if (query.exec()) {
        QMessageBox::information(this, "Sukces", "Status blokady użytkownika został zmieniony.");
        Database::logActivity(m_adminId, "ADMIN_ACTION", QString("Administrator zmienił status użytkownika o ID %1 na: %2").arg(userId).arg(newBlockStatus ? "ZABLOKOWANIE" : "ODBLOKOWANIE"));
        refreshData();
    } else {
        QMessageBox::critical(this, "Błąd", "Nie udało się zaktualizować statusu w bazie danych.");
    }
}