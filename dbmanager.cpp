#include "DBManager.h"
#include <QSqlError>
#include <QDebug>

DBManager::DBManager(const QString &host,
                     const QString &db,
                     const QString &user,
                     const QString &pass,
                     int port)
    : hostName(host),
    dbName(db),
    userName(user),
    password(pass),
    portNumber(port)
{
}

DBManager::~DBManager()
{
    if (db.isOpen()) {
        db.close();
        qDebug() << "Database connection closed.";
    }
}

bool DBManager::connect()
{
    // Avoid duplicate connections
    if (QSqlDatabase::contains("main_connection"))
        db = QSqlDatabase::database("main_connection");
    else
        db = QSqlDatabase::addDatabase("QPSQL", "main_connection");

    db.setHostName(hostName);
    db.setDatabaseName(dbName);
    db.setUserName(userName);
    db.setPassword(password);
    db.setPort(portNumber);

    if (!db.open()) {
        qDebug() << "❌ Database connection failed:" << db.lastError().text();
        return false;
    }

    qDebug() << "✅ Connected to database successfully!";
    return true;
}

QSqlDatabase& DBManager::database()
{
    return db;
}

bool DBManager::isConnected() const
{
    return db.isOpen();
}
