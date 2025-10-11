#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dbmanager.h"
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

    // Prepare query to check user credentials
    QSqlQuery query(db.database());
    query.prepare("SELECT role_id FROM users WHERE username=:username AND password_hash=:password");
    query.bindValue(":username", username);
    query.bindValue(":password", password); // Replace with hashed password in production

    if (!query.exec()) {
        ui->labelStatus->setText("❌ Query failed: " + query.lastError().text());
        return;
    }

    if (query.next()) {
        int roleId = query.value(0).toInt();
        switch (roleId) {
        case 1:
            ui->labelStatus->setText("✅ Admin login successful!");
            // TODO: Open Admin Dashboard window
            break;
        case 2:
            ui->labelStatus->setText("👨‍🏫 Faculty login successful!");
            // TODO: Open Faculty Dashboard window
            break;
        case 3:
            ui->labelStatus->setText("🎓 Student login successful!");
            // TODO: Open Student Dashboard window
            break;
        default:
            ui->labelStatus->setText("⚠️ Unknown role ID!");
            break;
        }
    } else {
        ui->labelStatus->setText("❌ Invalid username or password.");
    }
}
