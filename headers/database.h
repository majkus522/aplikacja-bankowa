#pragma once

#include <QSqlDatabase>
#include <QString>

class Database
{
    public:
        Database() = delete;
        static bool initDatabase(const QString& host, const QString& dbName, const QString& user, const QString& password, int port = 5432);
        static void closeDatabase();
        static bool logActivity(int userId, const QString &actionType, const QString &description);
};