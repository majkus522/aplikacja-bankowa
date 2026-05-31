#include "bankapp.h"
#include "database.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Inicjalizujemy bazę danych na samym starcie aplikacji
    bool dbConnected = Database::initDatabase(
        "localhost",       // Host
        "aplikacja bankowa",      // Nazwa bazy danych
        "postgres",        // Użytkownik
        "P@ssw0rd"      // Hasło
    );

    if (!dbConnected) {
        QMessageBox::critical(nullptr, "Błąd krytyczny", 
                              "Nie można uruchomić aplikacji bez połączenia z bazą danych.");
        return 1; 
    }

    BankApp w;
    w.show();

    int result = a.exec();

    // Po zamknięciu pętli zdarzeń Qt sprzątamy połączenie
    Database::closeDatabase();

    return result;
}