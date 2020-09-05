#ifndef QLITEMVVM_TREE_WIDGET_H
#define QLITEMVVM_TREE_WIDGET_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QObject>
#include <QWidget>

#include "../DependencyObject/DependencyObject"

class ObjectTreeItem : public QTreeWidgetItem
{
public:
    ObjectTreeItem(LiteObject * object, const QStringList& str, ObjectTreeItem* parent=nullptr);
    LiteObject* obj;
    static ObjectTreeItem* CreateTopObjectTreeItem(LiteObject* root);
};

class ObjectTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ObjectTreeWidget(QWidget * parent=nullptr);
    Q_SLOT void createTreeList();
};

#endif