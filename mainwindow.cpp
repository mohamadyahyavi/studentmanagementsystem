#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dbmanager.h"
#include "admindashboard.h"
#include "facultydashboard.h"
#include "studentdashboard.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QTimer>
#include <QEventLoop>

extern DBManager db; // global DBManager (ensure it's defined in main.cpp)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Slot called when the login button is clicked
void MainWindow::on_pushButtonLogin_clicked()
{
    QString username = ui->lineEditUsername->text();
    QString password = ui->lineEditPassword->text();

    if (username.isEmpty() || password.isEmpty()) {
        ui->labelStatus->setText("⚠️ Please enter both username and password.");
        return;
    }

    // Query both role_id and person_id
    QSqlQuery query(db.database());
    query.prepare("SELECT role_id, person_id FROM users WHERE username = :username AND password_hash = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password); // TODO: replace with hashed password

    if (!query.exec()) {
        ui->labelStatus->setText("❌ Query failed: " + query.lastError().text());
        qDebug() << "SQL Error:" << query.lastError().text();
        return;
    }

    if (query.next()) {
        int roleId = query.value(0).toInt();
        int personId = query.value(1).toInt();

        qDebug() << "Login successful! Role ID:" << roleId << "Person ID:" << personId;
       // QEventLoop loop;
        //QTimer::singleShot(5000, &loop, &QEventLoop::quit); // 5000 ms = 5 seconds
        //loop.exec();
        this->hide(); // hide login window

        // Open the correct dashboard based on role
        switch (roleId) {
        case 1: { // Admin
            AdminDashboard *admin = new AdminDashboard(personId);
            admin->show();
            break;
        }
        case 2: { // Faculty
            FacultyDashboard *faculty = new FacultyDashboard(personId);
            faculty->show();
            break;
        }
        case 3: { // Student
            StudentDashboard *student = new StudentDashboard(personId);
            student->show();
            break;
        }
        default:
            ui->labelStatus->setText("⚠️ Unknown role ID!");
            this->show();
            break;
        }
    } else {
        ui->labelStatus->setText("❌ Invalid username or password.");
        qDebug() << "Login failed for username:" << username;
    }
}
