#pragma once

#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QDoubleValidator>

class GradeDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit GradeDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
};
