#ifndef FACULTYDASHBOARD_H
#define FACULTYDASHBOARD_H

#include <QWidget>
#include "editablequerymodel.h"  // Your custom editable model

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
    int m_personId;                     // person_id from users table (FK to faculty.faculty_id)
    EditableQueryModel *studentsModel;  // Model to hold students + grades

    void loadCourses();                 // Load courses into comboBoxCourses
    void loadStudentsForCourse(int courseId); // Load students & grades for selected course
};

#endif // FACULTYDASHBOARD_H

