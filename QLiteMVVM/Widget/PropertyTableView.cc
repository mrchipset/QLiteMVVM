#include "PropertyTableView.h"
#include <QLabel>
#include <QDebug>
#include <QDialog>
#include <QPushButton>
#include <QMessageBox>

#pragma region class PropertyItemDelegate
PropertyItemDelegate::PropertyItemDelegate(QObject *parent) : QItemDelegate(parent)
{
}

QWidget *PropertyItemDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &option,
    const QModelIndex &index) const
{
    

    if(index.column() == Name)  //Property Name
    {
        QLabel* editor = new QLabel(parent);
        editor->setStyleSheet("QLabel { background-color : white; color : black; }");
        return editor;
    }
    else    //Property Value
    {
        //Get the type of property value
        const PropertyItemModel* model = qobject_cast<const PropertyItemModel*>(index.model());
        if (model != nullptr)
        {
            qDebug() << model->itemFromIndex(index)->data(Qt::EditRole).type();
        }
        return QItemDelegate::createEditor(parent, option, index);
    }
}

void PropertyItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == Name)
    {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        static_cast<QLabel*>(editor)->setText(value);
    }
    else
    {
        QItemDelegate::setEditorData(editor, index);
    }
    
}

void PropertyItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.column() == Name)
    {
        QString value = static_cast<QLabel*>(editor)->text();
        model->setData(index, value, Qt::EditRole);
    }
    else
    {
        QItemDelegate::setModelData(editor, model, index);
    }
}

void PropertyItemDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option,
    const QModelIndex &index) const
{
    QItemDelegate::updateEditorGeometry(editor, option, index);
}
#pragma endregion

#pragma region class PropertyItemModel

PropertyItemModel::PropertyItemModel(QObject* parent) : QStandardItemModel(parent)
{

}

#pragma endregion

#pragma region class PropertyItem
PropertyItem::PropertyItem() : QStandardItem()
{
    
}
#pragma endregion

#pragma region class PropertyTableView
PropertyTableView::PropertyTableView(QWidget *parent) : QTableView(parent)
{

}
#pragma endregion