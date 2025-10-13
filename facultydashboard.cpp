#include "facultydashboard.h"
#include "ui_facultydashboard.h"

FacultyDashboard::FacultyDashboard(int personId,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FacultyDashboard)
{
    ui->setupUi(this);
}

FacultyDashboard::~FacultyDashboard()
{
    delete ui;
}
