#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dbmanager.h"
#include "admindashboard.h"
#include "facultydashboard.h"
#include "studentdashboard.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

extern DBManager db; // Make sure you have a global DBManager in main.cpp

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

    QSqlQuery query(db.database());
    query.prepare("SELECT role_id FROM users WHERE username=:username AND password_hash=:password");
    query.bindValue(":username", username);
    query.bindValue(":password", password); // replace with hashed password later

    if (!query.exec()) {
        ui->labelStatus->setText("❌ Query failed: " + query.lastError().text());
        return;
    }

    if (query.next()) {
        int roleId = query.value(0).toInt();

        this->hide(); // hide login window

        switch (roleId) {
        case 1: {
            AdminDashboard *admin = new AdminDashboard();
            admin->show();
            break;
        }
        case 2: {
            FacultyDashboard *faculty = new FacultyDashboard();
            faculty->show();
            break;
        }
        case 3: {
            StudentDashboard *student = new StudentDashboard();
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
    }
}
