#include "admindashboard.h"
#include "ui_admindashboard.h"

AdminDashboard::AdminDashboard(int personId, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdminDashboard)
{
    ui->setupUi(this);

    // Example: use personId
    // You can store it in a member variable if needed
    // e.g., this->personId = personId;

    // For demonstration, let's print personId to console
    qDebug() << "Logged in Admin ID:" << personId;

    // TODO: load admin data based on personId
}

AdminDashboard::~AdminDashboard()
{
    delete ui;
}

void AdminDashboard::on_comboBoxAction_currentIndexChanged(int index)
{

}

