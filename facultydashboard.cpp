#include "facultydashboard.h"
#include "ui_facultydashboard.h"
#include "dbmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

extern DBManager db;

FacultyDashboard::FacultyDashboard(int personId, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FacultyDashboard)
    , m_personId(personId)
    , studentsModel(new QSqlQueryModel(this))
{
    ui->setupUi(this);

    // Load all courses taught by this faculty
    loadCourses();

    // Connect combobox selection to slot
    connect(ui->comboBoxCourses, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &FacultyDashboard::onCourseChanged);
}

FacultyDashboard::~FacultyDashboard()
{
    delete ui;
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

    // Load students for the first course by default
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
    QSqlQuery query(db.database());
    query.prepare(R"(
        SELECT s.first_name || ' ' || s.last_name AS "Student",
               s.email AS "Email",
               s.phone_no AS "Phone",
               e.grade AS "Grade"
        FROM enrollment e
        JOIN student s ON e.student_id = s.student_id
        WHERE e.course_id = :courseId
        ORDER BY s.last_name, s.first_name
    )");
    query.bindValue(":courseId", courseId);

    if (!query.exec()) {
        qDebug() << "Failed to load students:" << query.lastError().text();
        return;
    }

    studentsModel->setQuery(query);
    ui->tableViewStudents->setModel(studentsModel);
    ui->tableViewStudents->resizeColumnsToContents();
}
