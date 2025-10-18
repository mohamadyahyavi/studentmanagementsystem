#pragma once

#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class EditableQueryModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit EditableQueryModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());

    // Make only "Grade" column editable
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Update database when grade is edited
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void setDb(QSqlDatabase db);

private:
    QSqlDatabase m_db;
};
