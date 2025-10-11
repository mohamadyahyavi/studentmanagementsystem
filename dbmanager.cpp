#include "DBManager.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

DBManager::DBManager(const QString &host, const QString &db, const QString &user, const QString &pass, int port)
    : hostName(host), dbName(db), userName(user), password(pass), portNumber(port) {}

DBManager::~DBManager() {
    if (db.isOpen()) db.close();
}

bool DBManager::connect() {
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(hostName);
    db.setDatabaseName(dbName);
    db.setUserName(userName);
    db.setPassword(password);
    db.setPort(portNumber);

    if (!db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
        return false;
    }
    qDebug() << "Connected to database successfully!";
    return true;
}

QSqlDatabase& DBManager::database() {
    return db;
}
