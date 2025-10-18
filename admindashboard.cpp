#include "admindashboard.h"
#include "ui_admindashboard.h"
#include "dbmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

extern DBManager db; // use the global database connection

AdminDashboard::AdminDashboard(int personId, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdminDashboard)
    , adminId(personId)
{
    ui->setupUi(this);

    qDebug() << "Logged in Admin ID:" << personId;

    // Set default page (Add Student)
    ui->stackedWidget->setCurrentIndex(0);

    // Connect combo box for action switching
    connect(ui->comboBoxAction, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AdminDashboard::on_comboBoxAction_currentIndexChanged);

    // Populate initial combo boxes
    populateDepartments();
    populateFacultyComboBoxes();
    populateStudentAndCourseCombos();
}

AdminDashboard::~AdminDashboard()
{
    delete ui;
}

// -----------------------------------------------------------------------------
// PAGE SWITCHING
// -----------------------------------------------------------------------------
void AdminDashboard::on_comboBoxAction_currentIndexChanged(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
    qDebug() << "Switched to page index:" << index;
}

// -----------------------------------------------------------------------------
// POPULATION FUNCTIONS
// -----------------------------------------------------------------------------
void AdminDashboard::populateDepartments()
{
    QSqlQuery query(db.database());
    if (!query.exec("SELECT dep_id, name FROM department")) return;

    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();

        ui->comboBoxStudentDepartment->addItem(name, id);
        ui->comboBoxFacultyDepartment->addItem(name, id);
        ui->comboBoxCourseDepartment->addItem(name, id);
    }
}

void AdminDashboard::populateFacultyComboBoxes()
{
    QSqlQuery query(db.database());
    if (!query.exec("SELECT faculty_id, first_name || ' ' || last_name FROM faculty")) return;

    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        ui->comboBoxCourseFaculty->addItem(name, id);
    }
}

void AdminDashboard::populateStudentAndCourseCombos()
{
    // Populate students
    QSqlQuery studentQuery(db.database());
    if (studentQuery.exec("SELECT student_id, first_name || ' ' || last_name FROM student")) {
        while (studentQuery.next()) {
            ui->comboBoxAssignStudent->addItem(studentQuery.value(1).toString(),
                                               studentQuery.value(0).toInt());
        }
    }

    // Populate courses
    QSqlQuery courseQuery(db.database());
    if (courseQuery.exec("SELECT course_id, name FROM course")) {
        while (courseQuery.next()) {
            ui->comboBoxAssignCourse->addItem(courseQuery.value(1).toString(),
                                              courseQuery.value(0).toInt());
        }
    }
}

// -----------------------------------------------------------------------------
// PAGE 1: ADD STUDENT
// -----------------------------------------------------------------------------
void AdminDashboard::on_pushButtonAddStudent_clicked()
{
    QString firstName = ui->lineEditStudentFirstName->text();
    QString lastName  = ui->lineEditStudentLastName->text();
    QString email     = ui->lineEditStudentEmail->text();
    QString phone     = ui->lineEditStudentPhone->text();
    QString gender    = ui->comboBoxStudentGender->currentText();
    QString address   = ui->textEditStudentAddress->toPlainText();
    int depId         = ui->comboBoxStudentDepartment->currentData().toInt();

    if (firstName.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter at least first name and email.");
        return;
    }

    QSqlDatabase database = db.database();
    database.transaction();

    QSqlQuery query(database);
    query.prepare(R"(
        INSERT INTO student (first_name, last_name, email, phone_no, gender, address, dep_id)
        VALUES (:first_name, :last_name, :email, :phone_no, :gender, :address, :dep_id)
        RETURNING student_id
    )");

    query.bindValue(":first_name", firstName);
    query.bindValue(":last_name", lastName);
    query.bindValue(":email", email);
    query.bindValue(":phone_no", phone);
    query.bindValue(":gender", gender);
    query.bindValue(":address", address);
    query.bindValue(":dep_id", depId);

    if (!query.exec() || !query.next()) {
        QMessageBox::critical(this, "Error", "Failed to add student: " + query.lastError().text());
        database.rollback();
        return;
    }

    int studentId = query.value(0).toInt();

    // Add user account for student
    QSqlQuery queryUser(database);
    queryUser.prepare(R"(
        INSERT INTO users (username, password_hash, role_id, person_id)
        VALUES (:username, :password_hash, 3, :person_id)
    )");

    queryUser.bindValue(":username", email);
    queryUser.bindValue(":password_hash", "123456"); // TODO: Hash password
    queryUser.bindValue(":person_id", studentId);

    if (!queryUser.exec()) {
        QMessageBox::critical(this, "Error", "Failed to create user for student: " + queryUser.lastError().text());
        database.rollback();
        return;
    }

    database.commit();
    QMessageBox::information(this, "Success", "Student and user account created successfully!");
}

// -----------------------------------------------------------------------------
// PAGE 2: ADD FACULTY
// -----------------------------------------------------------------------------
void AdminDashboard::on_pushButtonAddFaculty_clicked()
{
    QString firstName = ui->lineEditFacultyFirstName->text();
    QString lastName  = ui->lineEditFacultyLastName->text();
    QString email     = ui->lineEditFacultyEmail->text();
    QString phone     = ui->lineEditFacultyPhone->text();
    int depId         = ui->comboBoxFacultyDepartment->currentData().toInt();

    if (firstName.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter at least first name and email.");
        return;
    }

    QSqlDatabase database = db.database();
    database.transaction();

    QSqlQuery query(database);
    query.prepare(R"(
        INSERT INTO faculty (first_name, last_name, email, phone_no, dep_id)
        VALUES (:first_name, :last_name, :email, :phone_no, :dep_id)
        RETURNING faculty_id
    )");

    query.bindValue(":first_name", firstName);
    query.bindValue(":last_name", lastName);
    query.bindValue(":email", email);
    query.bindValue(":phone_no", phone);
    query.bindValue(":dep_id", depId);

    if (!query.exec() || !query.next()) {
        QMessageBox::critical(this, "Error", "Failed to add faculty: " + query.lastError().text());
        database.rollback();
        return;
    }

    int facultyId = query.value(0).toInt();

    // Add user account for faculty
    QSqlQuery queryUser(database);
    queryUser.prepare(R"(
        INSERT INTO users (username, password_hash, role_id, person_id)
        VALUES (:username, :password_hash, 2, :person_id)
    )");

    queryUser.bindValue(":username", email);
    queryUser.bindValue(":password_hash", "123456"); // TODO: Hash password
    queryUser.bindValue(":person_id", facultyId);

    if (!queryUser.exec()) {
        QMessageBox::critical(this, "Error", "Failed to create user for faculty: " + queryUser.lastError().text());
        database.rollback();
        return;
    }

    database.commit();
    QMessageBox::information(this, "Success", "Faculty and user account created successfully!");
}

// -----------------------------------------------------------------------------
// PAGE 3: CREATE COURSE
// -----------------------------------------------------------------------------
void AdminDashboard::on_pushButtonCreateCourse_clicked()
{
    QString name = ui->lineEditCourseName->text();
    QString code = ui->lineEditCourseCode->text();
    int credits = ui->spinBoxCourseCredits->value();
    int depId = ui->comboBoxCourseDepartment->currentData().toInt();
    int facultyId = ui->comboBoxCourseFaculty->currentData().toInt();

    if (name.isEmpty() || code.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Course name and code are required.");
        return;
    }

    QSqlQuery query(db.database());
    query.prepare(R"(
        INSERT INTO course (name, code, credits, dep_id, faculty_id)
        VALUES (:name, :code, :credits, :dep_id, :faculty_id)
    )");

    query.bindValue(":name", name);
    query.bindValue(":code", code);
    query.bindValue(":credits", credits);
    query.bindValue(":dep_id", depId);
    query.bindValue(":faculty_id", facultyId);

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to create course: " + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Success", "Course created successfully!");
}

// -----------------------------------------------------------------------------
// PAGE 4: ENROLL STUDENT TO COURSE
// -----------------------------------------------------------------------------
void AdminDashboard::on_pushButtonAssignStudentToCourse_clicked()
{
    int studentId = ui->comboBoxAssignStudent->currentData().toInt();
    int courseId  = ui->comboBoxAssignCourse->currentData().toInt();

    if (studentId == 0 || courseId == 0) {
        QMessageBox::warning(this, "Input Error", "Please select both student and course.");
        return;
    }

    QSqlQuery query(db.database());
    query.prepare("INSERT INTO enrollment (student_id, course_id) VALUES (:student_id, :course_id)");
    query.bindValue(":student_id", studentId);
    query.bindValue(":course_id", courseId);

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to enroll student: " + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Success", "Student enrolled successfully!");
}
