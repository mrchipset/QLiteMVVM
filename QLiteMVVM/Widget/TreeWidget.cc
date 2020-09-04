#include "TreeWidget.h"

#pragma region class ObjectTreeItem

ObjectTreeItem::ObjectTreeItem(LiteObject * object, const QStringList& str, ObjectTreeItem* parent) : QTreeWidgetItem(parent, str),
    obj(object)
{

}

static QStringList GetLiteObjectInfo(LiteObject* object)
{
    QStringList str;
    str << object->name() << object->metaObject()->className();
    return str;
};

static void WalkAndCreateTreeItem(ObjectTreeItem* parentItem, LiteObject* parentObj)
{


    if (parentObj != nullptr)
         {
            QObjectList childrens = parentObj->children();
            for(auto iter=childrens.begin();iter!=childrens.end();++iter)
            {
                LiteObject * liteObj = qobject_cast<LiteObject*>(*iter);
                if (liteObj)
                {
                    ObjectTreeItem* item = new ObjectTreeItem(liteObj, GetLiteObjectInfo(liteObj), parentItem);
                    WalkAndCreateTreeItem(item, liteObj);
                }
            }
         }
}

ObjectTreeItem* ObjectTreeItem::CreateTopObjectTreeItem(LiteObject* root)
{
    ObjectTreeItem* top = new ObjectTreeItem(root, GetLiteObjectInfo(root));
    WalkAndCreateTreeItem(top, root);
    return top;
    
}
#pragma endregion

#pragma region class ObjectTreeWidget

ObjectTreeWidget::ObjectTreeWidget(QWidget *parent) : QTreeWidget(parent)
{
    QStringList headers;
    headers << tr("ObjName") << tr("Type");
    this->setHeaderLabels(headers);
    ObjectTreeItem* rootItem = ObjectTreeItem::CreateTopObjectTreeItem(&LiteObject::CreateRootObject());
    this->addTopLevelItem(rootItem);
}

#pragma endregion