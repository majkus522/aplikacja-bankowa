#include "database.h"
#include <QSqlError>
#include <QDebug>

bool Database::initDatabase(const QString& host, const QString& dbName, 
                                   const QString& user, const QString& password, 
                                   int port) {
    // Sprawdzamy, czy połączenie już nie istnieje, aby nie tworzyć go dublować
    QSqlDatabase db;
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection)) {
        db = QSqlDatabase::database(QSqlDatabase::defaultConnection);
    } else {
        db = QSqlDatabase::addDatabase("QPSQL");
    }

    db.setHostName(host);
    db.setDatabaseName(dbName);
    db.setUserName(user);
    db.setPassword(password);
    db.setPort(port);

    if (!db.open()) {
        qCritical() << "Błąd otwierania bazy danych:" << db.lastError().text();
        return false;
    }

    qDebug() << "Połączenie z bazą danych PostgreSQL ustanowione pomyślnie.";
    return true;
}

void Database::closeDatabase() {
    QString connectionName = QSqlDatabase::defaultConnection;
    if (QSqlDatabase::contains(connectionName)) {
        {
            // Blok kodu gwarantuje, że obiekt 'db' zostanie zniszczony 
            // przed wywołaniem removeDatabase (wymóg Qt)
            QSqlDatabase db = QSqlDatabase::database(connectionName);
            if (db.isOpen()) {
                db.close();
            }
        }
        QSqlDatabase::removeDatabase(connectionName);
    }
}