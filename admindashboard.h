#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include <QWidget>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include "dbmanager.h" // Make sure you have access to your DBManager

namespace Ui {
class AdminDashboard;
}

class AdminDashboard : public QWidget
{
    Q_OBJECT

public:
    explicit AdminDashboard(int personId, QWidget *parent = nullptr);
    ~AdminDashboard();

private slots:
    void on_comboBoxAction_currentIndexChanged(int index);

    // Slots for page actions
    void on_pushButtonAddStudent_clicked();
    void on_pushButtonAddFaculty_clicked();
    void on_pushButtonCreateCourse_clicked();
    void on_pushButtonAssignStudentToCourse_clicked();

private:
    void populateDepartments();
    void populateFacultyComboBoxes();
    void populateStudentAndCourseCombos();
    Ui::AdminDashboard *ui;
    int adminId; // store logged-in admin ID if needed
};

#endif // ADMINDASHBOARD_H
