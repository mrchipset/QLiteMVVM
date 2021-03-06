#include "Object.h"
#include <QCoreApplication>
#include <QDebug>
#include <QMetaMethod>
#include <QMetaType>
#include <QMap>

#include "Property.h"

# pragma region class LitePropertyEvent

static void WalkAndCreateStringTree(QString& str, LiteObject* parentObj)
{
    if (parentObj != nullptr)
    {
        QObjectList childrens = parentObj->children();
        for(auto iter=childrens.begin();iter!=childrens.end();++iter)
        {
            LiteObject * liteObj = qobject_cast<LiteObject*>(*iter);
            if (liteObj)
            {
                str.append(liteObj->name());
                str.append("\n");
                WalkAndCreateStringTree(str, liteObj);
            }
        }
    }
}

const QEvent::Type LitePropertyChangedEvent::eventType =
    static_cast<QEvent::Type>(QEvent::registerEventType(LiteEvent::LitePropertyChanged));

LitePropertyChangedEvent::LitePropertyChangedEvent(LiteObject* sender,
        const QString& propertyName) : QEvent(eventType),
    m_sender(sender),
    m_propertyName(propertyName)
{
    setAccepted(false);
}

LitePropertyChangedEvent::~LitePropertyChangedEvent()
{
    qDebug() << "Destroy LiteEvent";
}

QString LitePropertyChangedEvent::propertyName()
{
    return m_propertyName;
}

LiteObject* LitePropertyChangedEvent::sender()
{
    return m_sender;
}

#pragma endregion class LitePropertyEvent

# pragma region class LiteObject

LiteObject LiteObject::RootObject("root");
bool metaTypeRegistered = false;

/**
 * @brief register Qt MetaType
 * 
 */
void registerMetaType()
{
    if (metaTypeRegistered)
    {
        return;
    }

    qRegisterMetaType<QMap<QString, Property*>>("QMap<QString, Property*>");
    metaTypeRegistered = true;
    
}
LiteObject::LiteObject(LiteObject* parent) : LiteObject("None", parent)
{

}

LiteObject::LiteObject(const QString& objName, LiteObject* parent)
    : QObject(parent),
    m_ObjectName(objName),
    m_parentObject(parent),
    m_childrenAttacherTimer(0)
{
    if(this!=&RootObject && this->parent() == nullptr)
    {
        setParent(&RootObject);
        m_parentObject = &RootObject;
    }
    setObjectName(objName);
}

LiteObject::~LiteObject()
{
    if (m_childrenAttacherTimer > 0)
    {
        killTimer(m_childrenAttacherTimer);
    }
}

QString LiteObject::name() const
{
    return m_ObjectName;
}

LiteObject* LiteObject::rootObject() const
{
    return &RootObject;
}

LiteObject* LiteObject::parentObject() const
{
    return m_parentObject;
}

QString LiteObject::printChildren()
{
    QString str;
    WalkAndCreateStringTree(str, this);
    QList<QByteArray> dynamicNames = dynamicPropertyNames();
    str.append("-------dynamic property-------\n");
    str.append(QString::number(dynamicNames.size()));
    str.append("\n");
    for(auto iter=dynamicNames.begin();iter!=dynamicNames.end();++iter)
    {
        str.append(*iter);
        str.append("\n");
    }
    return str;
}

LiteObject* LiteObject::getChildren(const QString& name)
{
    QObjectList childrens = this->children();
    for(auto iter=childrens.begin();iter!=childrens.end();++iter)
    {
        LiteObject * liteObj = qobject_cast<LiteObject*>(*iter);
        if (liteObj)
        {
            if(liteObj->name() == name)
            {
                return liteObj;
            }
        }
    }
    return nullptr;
}

LiteObject& LiteObject::CreateRootObject()
{
    registerMetaType();
    return RootObject;
}

void LiteObject::attachChildrenToProperty()
{
    //if empty kill timer
    if(m_childrenAttacherTimer > 0  && m_unFullyConstructedChildren.size() == 0)
    {
        killTimer(m_childrenAttacherTimer);
        m_childrenAttacherTimer = 0;
    }

    for(int i = 0; i < m_unFullyConstructedChildren.size(); ++i)
    {
        auto attacher = m_unFullyConstructedChildren[i];
        LiteObject* object = qobject_cast<LiteObject*>(attacher->object);
        if (object)
        {
            this->setProperty(object->name().toLocal8Bit(), QVariant::fromValue<LiteObject*>(object));
            attacher->retry = 0;
        }else
        {
            attacher->retry--;
        }
    }

    for (int i = m_unFullyConstructedChildren.size() - 1; i >= 0; --i)
    {
        m_unFullyConstructedChildren.remove(i);
    }
}

void LiteObject::litePropertyChangedEvent(LitePropertyChangedEvent* ev)
{
    qDebug() << "PropertyChangedEvent" << ev->sender()->name() << "PropertyName:" << ev->propertyName();
}

bool LiteObject::event(QEvent* ev)
{
    QDynamicPropertyChangeEvent* dynamicEv = nullptr;
    LitePropertyChangedEvent* event;
    QChildEvent* childEvent;
    LiteObject* liteObject;
    QObject* object;
    switch (ev->type()) {
    case QEvent::DynamicPropertyChange:
        qDebug() << "DynamicPropertyChanged";
        dynamicEv = dynamic_cast<QDynamicPropertyChangeEvent*>(ev);
        if (dynamicEv != nullptr) {
            litePropertyChangedEvent(
                new LitePropertyChangedEvent(this, dynamicEv->propertyName())
            );
        }
        break;
    case LiteEvent::LitePropertyChanged:
        event = dynamic_cast<LitePropertyChangedEvent*>(ev);
        litePropertyChangedEvent(event);
        break;
    case QEvent::ChildAdded:
        childEvent = dynamic_cast<QChildEvent*>(ev);
        qDebug() << childEvent;
        liteObject = qobject_cast<LiteObject*>(childEvent->child());
        if (liteObject) {
            // childObject->m_propertyListener =
            //     new LiteObjectPropertyListener(childObject);
            this->setProperty(liteObject->name().toLocal8Bit(), QVariant::fromValue<LiteObject*>(liteObject));
            qDebug() << childEvent->child();
            emit(rootObject()->objectTreeUpdated());
        }else
        {
            object = childEvent->child();
            ChildrenAttacher* attacher = new ChildrenAttacher();
            attacher->object = object;
            attacher->retry = 3;
            m_unFullyConstructedChildren.append(attacher);
            if (m_childrenAttacherTimer < 1)
            {
                m_childrenAttacherTimer = startTimer(50);
            }
        }
        
        
        //TODO add child to a map and try to cast to LiteObject and add as an dynamic property
        qDebug() << childEvent->child();
    default:
        qDebug() << ev->type();
        return QObject::event(ev);
    }
    return true;
}

void LiteObject::timerEvent(QTimerEvent* ev)
{
    if(ev->timerId() == m_childrenAttacherTimer)
    {
        attachChildrenToProperty();
    }
}

#pragma endregion class LiteObject

#pragma region class Object
DataObject::DataObject(const QString& objName, LiteObject* parent) : LiteObject(objName, parent)
{

}
#pragma endregion
