#ifndef QLITEMVVM_PROPERTY_TABLE_VIEW_H
#define QLITEMVVM_PROPERTY_TABLE_VIEW_H

#include <QTableView>
#include <QItemDelegate>
#include <QStandardItemModel>
#include <QModelIndex>

class PropertyItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    enum PropertyItemRole
    {
        Name = 0,
        Value
    };
    Q_ENUMS(PropertyItemRole);
public:
    PropertyItemDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const override;
};

class PropertyItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    PropertyItemModel(QObject* parent=nullptr);
};

class PropertyItem : public QStandardItem
{
public:
    PropertyItem();
};

class PropertyTableView : public QTableView
{
    Q_OBJECT
public:
    PropertyTableView(QWidget *parent = nullptr);

};

#endif