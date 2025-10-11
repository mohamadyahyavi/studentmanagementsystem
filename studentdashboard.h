#ifndef STUDENTDASHBOARD_H
#define STUDENTDASHBOARD_H

#include <QWidget>

namespace Ui {
class StudentDashboard;
}

class StudentDashboard : public QWidget
{
    Q_OBJECT

public:
    explicit StudentDashboard(QWidget *parent = nullptr);
    ~StudentDashboard();

private:
    Ui::StudentDashboard *ui;
};

#endif // STUDENTDASHBOARD_H
