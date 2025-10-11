#include "mainwindow.h"
#include "DBManager.h"
#include <QApplication>
#include <QDebug>

DBManager db("localhost", "studentdb", "postgres", "muhammad99");
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    if (!db.connect()) {
        qDebug() << "Failed to connect to database!";
        return -1;
    }

    qDebug() << "Database connection established!";

    MainWindow w;
    w.show();

    return a.exec();
}
