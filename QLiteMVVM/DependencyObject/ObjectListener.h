#ifndef QLITEMVVM_OBJECT_LISTENER_H
#define QLITEMVVM_OBJECT_LISTENER_H

#include <QMetaMethod>
#include <QMetaObject>
#include <QMetaProperty>
#include <QObject>

class FakeObjectListener : public QObject
{
public:
    FakeObjectListener(QObject* target = nullptr);
    virtual int qt_metacall(QMetaObject::Call c, int id, void** arguments);
    static QList<QMetaProperty> GetMetaProperty(QObject* obj);
    static QList<QMetaMethod> GetMetaMethod(QObject* obj);
    static QList<QMetaMethod> GetMetaSignal(QObject* obj);
protected:
    QMap<int, int> m_map;
    QObject* m_target;

    virtual void bindToTarget();
};

#endif