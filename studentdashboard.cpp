#include "studentdashboard.h"
#include "ui_studentdashboard.h"
#include "dbmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

extern DBManager db;

StudentDashboard::StudentDashboard(int personId, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StudentDashboard)
    , m_personId(personId)
    , coursesModel(new QSqlQueryModel(this))
{
    ui->setupUi(this);

    // Retrieve the username for this student from the database
    QSqlQuery query(db.database());
    query.prepare("SELECT username FROM users WHERE person_id = :id");
    query.bindValue(":id", m_personId);

    if (query.exec() && query.next()) {
        m_username = query.value("username").toString();
    } else {
        qDebug() << "Failed to retrieve username for student_id" << m_personId << ":" << query.lastError().text();
        m_username = "";
    }

    loadStudentInfo();
    loadEnrolledCourses();
}

StudentDashboard::~StudentDashboard()
{
    delete ui;
}

void StudentDashboard::loadStudentInfo()
{
    qDebug() << "🔹 Loading student info for person_id:" << m_personId;

    QSqlQuery query(db.database());
    query.prepare(R"(
        SELECT s.first_name || ' ' || s.last_name AS fullname,
               s.email,
               s.phone_no
        FROM student s
        WHERE s.student_id = :id
    )");
    query.bindValue(":id", m_personId);

    if (query.exec()) {
        if (query.next()) {
            QString fullName = query.value("fullname").toString();
            QString email = query.value("email").toString();
            QString phone = query.value("phone_no").toString();

            qDebug() << "✅ Student info found:" << fullName << email << phone;

            ui->labelName->setText(fullName);
            ui->labelEmail->setText(email);
            ui->labelPhone->setText(phone);
        } else {
            qDebug() << "⚠️ No student found for person_id:" << m_personId;
        }
    } else {
        qDebug() << "❌ SQL Error in loadStudentInfo:" << query.lastError().text();
    }
}


void StudentDashboard::loadEnrolledCourses()
{
    qDebug() << "🔹 Loading courses for student_id:" << m_personId;

    QSqlQuery query(db.database());
    query.prepare(R"(
        SELECT c.name AS "Course",
               c.code AS "Code",
               f.first_name || ' ' || f.last_name AS "Instructor",
               e.grade
        FROM enrollment e
        JOIN course c ON e.course_id = c.course_id
        LEFT JOIN faculty f ON c.faculty_id = f.faculty_id
        WHERE e.student_id = :studentId
    )");
    query.bindValue(":studentId", m_personId);

    if (!query.exec()) {
        qDebug() << "❌ Failed to load courses:" << query.lastError().text();
        return;
    }

    coursesModel->setQuery(query);
    ui->tableViewCourses->setModel(coursesModel);
}



