#include "ObjectListener.h"
#include <QCoreApplication>
#include <QDebug>
#include <QVariant>

#include "Object.h"

#pragma region class FakeObjectListener

FakeObjectListener::FakeObjectListener(QObject* target) : QObject(target),
    m_target(target)
{
    bindToTarget();
}

int FakeObjectListener::qt_metacall(QMetaObject::Call c, int id, void** arguments)
{
    if (id < 0 || c != QMetaObject::InvokeMetaMethod)
        return id;
    // Get the signalMethod
    auto signalKey = id;
    auto signalId = m_map[signalKey];
    auto signalMethod = m_target->metaObject()->method(signalId);
    qDebug() << signalMethod.methodSignature();
    return -1;
}

QList<QMetaProperty> FakeObjectListener::GetMetaProperty(QObject* obj)
{
    QList<QMetaProperty> propertyList;
    const QMetaObject* metaObject = obj->metaObject();
    for (int i = 0; i < metaObject->propertyCount(); ++i) {
        QMetaProperty property = metaObject->property(i);
        propertyList.append(property);
        // qDebug() << property.typeName() << property.name()
        //          << property.notifySignal().methodSignature();
    }
    return propertyList;
}

QList<QMetaMethod> FakeObjectListener::GetMetaMethod(QObject* obj)
{
    QList<QMetaMethod> methodList;
    const QMetaObject* metaObject = obj->metaObject();
    for (int i = 0; i < metaObject->methodCount(); ++i) {
        QMetaMethod method = metaObject->method(i);
        methodList.append(method);
        // qDebug() << method.typeName() <<
        //          method.methodType() << method.methodSignature();
    }
    return methodList;
}

QList<QMetaMethod> FakeObjectListener::GetMetaSignal(QObject* obj)
{
    QList<QMetaMethod> methodList;
    const QMetaObject* metaObject = obj->metaObject();
    for (int i = 0; i < metaObject->methodCount(); ++i) {
        QMetaMethod method = metaObject->method(i);
        if (method.methodType() == QMetaMethod::Signal) {
            methodList.append(method);
            // qDebug() << method.typeName() <<
            //          method.methodType() << method.methodSignature();
        }

    }
    return methodList;
}

// TODO revise the bind and m_map to provide more information
void FakeObjectListener::bindToTarget()
{
    auto mo = m_target->metaObject();
    auto offset = mo->methodOffset();
    m_map.clear();
    for (auto i = offset; i != mo->methodCount(); ++i) {
        auto m = mo->method(i);
        if (m.methodType() != QMetaMethod::Signal)
            continue;
        m_map[i] = i;
        QMetaObject::connect(m_target, i, this, i, Qt::UniqueConnection);
    }
}

#pragma endregion

#pragma region class LiteObjectPropertyListener
LiteObjectPropertyListener::LiteObjectPropertyListener(QObject* target, LiteObject* binder) :
    FakeObjectListener(target),
    m_binder(binder)
{
    bindToTarget();
}

int LiteObjectPropertyListener::qt_metacall(QMetaObject::Call c, int id, void** arguments)
{
    if (id < 0 || c != QMetaObject::InvokeMetaMethod)
        return id;
    // Get the signalMethod
    auto signalKey = id;
    if (m_targetPropertyMap.contains(signalKey)) {
        QMetaProperty property = m_targetPropertyMap[signalKey];
        qDebug() << property.name() << m_target->property(property.name());
        // find property valid for two way/ one way binding.

    }
    return -1;
}

void LiteObjectPropertyListener::bindToTarget()
{
    m_propertyMap.clear();
    m_targetPropertyMap.clear();
    // Register all property with notification
    QList<QMetaProperty> propertyList = FakeObjectListener::GetMetaProperty(m_target);
    for (auto property : propertyList) {
        if (property.hasNotifySignal()) {
            // qDebug() << property.notifySignalIndex();
            // qDebug() << property.notifySignal().methodSignature();
            m_targetPropertyMap[property.notifySignalIndex()] = property;
        }
    }
    // Create valid property hash map
    //copy all valid property to binder
    copyProperty();
}

void LiteObjectPropertyListener::copyProperty()
{
    QList<QMetaProperty> targetPropertyList = FakeObjectListener::GetMetaProperty(m_target);
    // get all property names for binder including meta and dynamic
    QList<QMetaProperty> binderPropertyList = FakeObjectListener::GetMetaProperty(m_binder);
    QList<QByteArray> binderPropertyNames;
    for (auto property : binderPropertyList) {
        binderPropertyNames.append(property.name());
    }

    binderPropertyNames.append(m_binder->dynamicPropertyNames());
    for (auto property : targetPropertyList) {
        if (!binderPropertyNames.contains(property.name())) {
            QVariant var = m_target->property(property.name());
            m_binder->setProperty(property.name(), var);
        }
    }
}
#pragma endregion