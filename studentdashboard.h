#ifndef STUDENTDASHBOARD_H
#define STUDENTDASHBOARD_H

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
class StudentDashboard;
}

class StudentDashboard : public QWidget
{
    Q_OBJECT

public:
    explicit StudentDashboard(int personId, QWidget *parent = nullptr);
    ~StudentDashboard();

private:
    Ui::StudentDashboard *ui;
    QString m_username;          // Stores the student's username
    int m_personId;              // Stores the student's ID
    QSqlQueryModel *coursesModel;

    void loadStudentInfo();
    void loadEnrolledCourses();
};

#endif // STUDENTDASHBOARD_H
