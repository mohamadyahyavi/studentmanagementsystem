#ifndef FACULTYDASHBOARD_H
#define FACULTYDASHBOARD_H

#include <QWidget>

namespace Ui {
class FacultyDashboard;
}

class FacultyDashboard : public QWidget
{
    Q_OBJECT

public:
    explicit FacultyDashboard(QWidget *parent = nullptr);
    ~FacultyDashboard();

private:
    Ui::FacultyDashboard *ui;
};

#endif // FACULTYDASHBOARD_H
