#include "Object.h"
#include <QCoreApplication>
#include <QDebug>
#include <QMetaMethod>
# pragma region class LitePropertyEvent
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
LiteObject::LiteObject(const QString& objName, LiteObject* parent) : QObject(parent),
    m_ObjectName(objName),
    m_parentObject(parent)
{

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
    return &RootObject;
}

LiteObject* LiteObject::parentObject() const
{
    return m_parentObject;
}

LiteObject& LiteObject::CreateRootObject()
{
    return RootObject;
}

bool LiteObject::event(QEvent* ev)
{
    QDynamicPropertyChangeEvent* dynamicEv = nullptr;
    LitePropertyChangedEvent* event;
    switch (ev->type()) {
    case QEvent::DynamicPropertyChange:
        qDebug() << "DynamicPropertyChanged";
        dynamicEv = dynamic_cast<QDynamicPropertyChangeEvent*>(ev);
        if (dynamicEv != nullptr) {
            litePropertyChangedEvent(
                &LitePropertyChangedEvent(this, dynamicEv->propertyName())
            );
        }
        break;
    case LiteEvent::LitePropertyChanged:
        event = dynamic_cast<LitePropertyChangedEvent*>(ev);
        litePropertyChangedEvent(event);
        break;
    default:
        return QObject::event(ev);
    }
    return true;
}

void LiteObject::litePropertyChangedEvent(LitePropertyChangedEvent* ev)
{
    qDebug() << "PropertyChangedEvent" << ev->sender()->name() <<
             "PropertyName:" << ev->propertyName();
}

#pragma endregion class LiteObject

