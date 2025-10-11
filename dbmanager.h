#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QtSql/QSqlDatabase>
#include <QString>

class DBManager {
public:
    DBManager(const QString &host, const QString &db, const QString &user, const QString &pass, int port = 5432);
    ~DBManager();

    bool connect();
    QSqlDatabase& database();

private:
    QString hostName, dbName, userName, password;
    int portNumber;
    QSqlDatabase db;
};

#endif // DBMANAGER_H
