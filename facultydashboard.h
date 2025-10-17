#ifndef FACULTYDASHBOARD_H
#define FACULTYDASHBOARD_H

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
class FacultyDashboard;
}

class FacultyDashboard : public QWidget
{
    Q_OBJECT

public:
    explicit FacultyDashboard(int personId, QWidget *parent = nullptr);
    ~FacultyDashboard();

private:
    Ui::FacultyDashboard *ui;
    int m_personId;                    // Store the faculty's personId
    QSqlQueryModel *studentsModel;     // Model for students table

    // Helper functions
    void loadCourses();
    void onCourseChanged(int index);
    void loadStudentsForCourse(int courseId);
};

#endif // FACULTYDASHBOARD_H
