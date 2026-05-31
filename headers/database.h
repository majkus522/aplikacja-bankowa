#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QString>

class Database {
public:
    // Klasa statyczna nie powinna mieć możliwości tworzenia obiektów
    Database() = delete;

    // Statyczna metoda do inicjalizacji i otwarcia połączenia
    static bool initDatabase(const QString& host, const QString& dbName, 
                             const QString& user, const QString& password, 
                             int port = 5432);

    // Statyczna metoda do zamykania połączenia
    static void closeDatabase();
};

#endif // DATABASE_H