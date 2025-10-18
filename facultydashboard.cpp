#include "facultydashboard.h"
#include "ui_facultydashboard.h"
#include "dbmanager.h"
#include "gradedelegate.h"
#include "editablequerymodel.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QAbstractItemView>
#include <QSqlRecord>

extern DBManager db;

FacultyDashboard::FacultyDashboard(int personId, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FacultyDashboard)
    , m_personId(personId)
    , studentsModel(nullptr)
{
    ui->setupUi(this);

    // Load all courses taught by this faculty
    loadCourses();

    // Connect comboBox signal to course change
    connect(ui->comboBoxCourses, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &FacultyDashboard::onCourseChanged);

    // Enable editing via double click or selection
    ui->tableViewStudents->setEditTriggers(
        QAbstractItemView::DoubleClicked |
        QAbstractItemView::SelectedClicked
        );
}

FacultyDashboard::~FacultyDashboard()
{
    delete ui;
    if (studentsModel)
        delete studentsModel;
}

void FacultyDashboard::loadCourses()
{
    QSqlQuery query(db.database());
    query.prepare("SELECT course_id, name FROM course WHERE faculty_id = :facultyId ORDER BY name");
    query.bindValue(":facultyId", m_personId);

    if (!query.exec()) {
        qDebug() << "Failed to load courses:" << query.lastError().text();
        return;
    }

    ui->comboBoxCourses->clear();
    while (query.next()) {
        int courseId = query.value("course_id").toInt();
        QString courseName = query.value("name").toString();
        ui->comboBoxCourses->addItem(courseName, courseId);
    }

    // Load students for first course by default
    if (ui->comboBoxCourses->count() > 0)
        loadStudentsForCourse(ui->comboBoxCourses->currentData().toInt());
}

void FacultyDashboard::onCourseChanged(int index)
{
    if (index >= 0)
        loadStudentsForCourse(ui->comboBoxCourses->itemData(index).toInt());
}

void FacultyDashboard::loadStudentsForCourse(int courseId)
{
    if (studentsModel)
        delete studentsModel;

    // Use EditableQueryModel to allow database updates
    studentsModel = new EditableQueryModel(this, db.database());

    // --- DO NOT CHANGE YOUR ORIGINAL QUERY ---
    QString queryStr = QString(R"(
        SELECT enrollment.enrollment_id,
               student.first_name || ' ' || student.last_name AS "Student",
               course.name AS "Course",
               enrollment.grade AS "Grade"
        FROM enrollment
        JOIN student ON enrollment.student_id = student.student_id
        JOIN course ON enrollment.course_id = course.course_id
        WHERE enrollment.course_id = %1
        ORDER BY student.last_name, student.first_name
    )").arg(courseId);

    studentsModel->setQuery(queryStr, db.database());

    if (studentsModel->lastError().isValid()) {
        qDebug() << "Failed to load students:" << studentsModel->lastError().text();
        return;
    }

    ui->tableViewStudents->setModel(studentsModel);

    // Hide enrollment_id column
    int enrollmentCol = studentsModel->record().indexOf("enrollment_id");
    if (enrollmentCol != -1)
        ui->tableViewStudents->hideColumn(enrollmentCol);

    // Rename headers
    studentsModel->setHeaderData(studentsModel->record().indexOf("Student"), Qt::Horizontal, "Student Name");
    studentsModel->setHeaderData(studentsModel->record().indexOf("Course"), Qt::Horizontal, "Course Name");
    studentsModel->setHeaderData(studentsModel->record().indexOf("Grade"), Qt::Horizontal, "Grade");

    // Apply GradeDelegate for editable column
    int gradeCol = studentsModel->record().indexOf("Grade");
    if (gradeCol != -1)
        ui->tableViewStudents->setItemDelegateForColumn(gradeCol, new GradeDelegate(this));

    ui->tableViewStudents->resizeColumnsToContents();

    // Ensure the table view allows editing
    ui->tableViewStudents->setEditTriggers(
        QAbstractItemView::DoubleClicked |
        QAbstractItemView::SelectedClicked
        );
}
