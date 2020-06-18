#include "ObjectListener.h"
#include <QDebug>

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
        qDebug() << property.typeName() << property.name()
                 << property.notifySignal().methodSignature();
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
        qDebug() << method.typeName() <<
                 method.methodType() << method.methodSignature();
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
            qDebug() << method.typeName() <<
                     method.methodType() << method.methodSignature();
        }

    }
    return methodList;
}

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