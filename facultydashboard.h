#pragma once

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

private slots:
    void onCourseChanged(int index);  // Triggered when the selected course changes

private:
    Ui::FacultyDashboard *ui;
    int m_personId;                    // person_id from users table (FK to faculty.faculty_id)
    QSqlQueryModel *studentsModel;     // Model to hold students and grades

    void loadCourses();                // Load courses into comboBoxCourses
    void loadStudentsForCourse(int courseId); // Load students & grades for selected course
};
