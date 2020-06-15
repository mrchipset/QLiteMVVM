#include "Object.h"

const QEvent::Type LitePropertyChangedEvent::eventType =
    static_cast<QEvent::Type>(QEvent::registerEventType(QEvent::User + 1));

LitePropertyChangedEvent::LitePropertyChangedEvent(LiteObject * sender,
 const QString& propertyName) : QEvent(eventType),
    m_sender(sender),
    m_propertyName(propertyName)
 {
 }

 
LiteObject::LiteObject(const QString& objName, LiteObject * parent) : QObject(parent),
    m_ObjectName(objName),
    m_parentObject(parent)
{
    m_rootObject = m_parentObject->rootObject();
}

LiteObject::~LiteObject()
{

}

QString LiteObject::name() const
{
    return m_ObjectName;
}

LiteObject* LiteObject::rootObject() const
{
   return m_rootObject;
}

LiteObject* LiteObject::parentObject() const
{
    return m_parentObject;
}

LiteObject * LiteObject::CreateRootObject()
{
    //TODO Create root object
    //TODO revise the private m_rootObject member to with static annotation
    return nullptr;
}

bool LiteObject::event(QEvent * ev)
{
    return QObject::event(ev);
}