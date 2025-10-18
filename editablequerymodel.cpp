#include "editablequerymodel.h"
#include <QSqlRecord>

EditableQueryModel::EditableQueryModel(QObject *parent, QSqlDatabase db)
    : QSqlQueryModel(parent), m_db(db)
{
}

void EditableQueryModel::setDb(QSqlDatabase db)
{
    m_db = db;
}

Qt::ItemFlags EditableQueryModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f = QSqlQueryModel::flags(index);

    if (index.column() == this->record().indexOf("Grade"))
        f |= Qt::ItemIsEditable;

    return f;
}

bool EditableQueryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    int gradeCol = record().indexOf("Grade");
    if (index.column() != gradeCol)
        return false;

    int enrollmentCol = record().indexOf("enrollment_id");
    int enrollmentId = data(this->index(index.row(), enrollmentCol)).toInt();

    QSqlQuery query(m_db);
    query.prepare("UPDATE enrollment SET grade = :grade WHERE enrollment_id = :enrollmentId");
    query.bindValue(":grade", value);
    query.bindValue(":enrollmentId", enrollmentId);

    if (!query.exec()) {
        qDebug() << "Failed to update grade:" << query.lastError().text();
        return false;
    }

    // Update the model view
    return QSqlQueryModel::setData(index, value, role);
}
