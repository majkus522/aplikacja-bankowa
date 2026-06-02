#include "headers/bankapp.h"
#include "headers/database.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    bool dbConnected = Database::initDatabase(
        "localhost",
        "aplikacja bankowa",
        "postgres",
        "P@ssw0rd"
    );
    if (!dbConnected)
    {
        QMessageBox::critical(nullptr, "Błąd krytyczny", "Nie można uruchomić aplikacji bez połączenia z bazą danych.");
        return 1; 
    }
    BankApp w;
    w.show();
    int result = a.exec();
    Database::closeDatabase();
    return result;
}