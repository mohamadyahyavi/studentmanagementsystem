#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QtSql/QSqlDatabase>
#include <QString>

/**
 * @brief The DBManager class handles the PostgreSQL connection for the entire application.
 *
 * It uses the Qt SQL module (QPSQL driver) to connect to a PostgreSQL database.
 * The same connection can safely be shared across multiple classes (via extern DBManager db).
 */
class DBManager {
public:
    /**
     * @brief Constructs a DBManager with given connection parameters.
     * @param host Database server host (e.g., "localhost")
     * @param db Database name
     * @param user Username for authentication
     * @param pass Password for authentication
     * @param port Port number (default 5432)
     */
    explicit DBManager(const QString &host = "localhost",
                       const QString &db = "studentdb",
                       const QString &user = "postgres",
                       const QString &pass = "",
                       int port = 5432);

    /**
     * @brief Destructor — safely closes the database connection.
     */
    ~DBManager();

    /**
     * @brief Connects to the PostgreSQL database using QPSQL driver.
     * @return true if the connection was successful, false otherwise.
     */
    bool connect();

    /**
     * @brief Returns a reference to the QSqlDatabase instance for executing queries.
     */
    QSqlDatabase& database();

    /**
     * @brief Checks if the database connection is open and valid.
     * @return true if connected, false otherwise.
     */
    bool isConnected() const;

private:
    QString hostName;
    QString dbName;
    QString userName;
    QString password;
    int portNumber;

    QSqlDatabase db;
};

#endif // DBMANAGER_H
