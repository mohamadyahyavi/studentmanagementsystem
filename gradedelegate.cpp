#include "gradedelegate.h"
#include <QLineEdit>

GradeDelegate::GradeDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *GradeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &,
                                     const QModelIndex &) const
{
    QLineEdit *editor = new QLineEdit(parent);
    editor->setValidator(new QDoubleValidator(0, 100, 2, editor)); // Grades 0-100
    return editor;
}

void GradeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(value);
}

void GradeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    QString newGrade = lineEdit->text();

    // Only update the model; EditableQueryModel handles database
    model->setData(index, newGrade, Qt::EditRole);
}

void GradeDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                         const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}
