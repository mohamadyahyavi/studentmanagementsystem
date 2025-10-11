#include "studentdashboard.h"
#include "ui_studentdashboard.h"

StudentDashboard::StudentDashboard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StudentDashboard)
{
    ui->setupUi(this);
}

StudentDashboard::~StudentDashboard()
{
    delete ui;
}
