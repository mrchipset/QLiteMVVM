#ifndef QLITEMVVM_OBJECT_LISTENER_H
#define QLITEMVVM_OBJECT_LISTENER_H

#include <QMetaMethod>
#include <QMetaObject>
#include <QMetaProperty>
#include <QObject>

class LiteObject;

class FakeObjectListener : public QObject
{
public:
    FakeObjectListener(QObject* target);
    virtual int qt_metacall(QMetaObject::Call c, int id, void** arguments);
    static QList<QMetaProperty> GetMetaProperty(QObject* obj);
    static QList<QMetaMethod> GetMetaMethod(QObject* obj);
    static QList<QMetaMethod> GetMetaSignal(QObject* obj);
protected:
    QMap<int, int> m_map;
    QObject* m_target;
    virtual void bindToTarget();
};

class LiteObjectPropertyListener : public FakeObjectListener
{
public:
    LiteObjectPropertyListener(QObject* target, LiteObject* binder);
    virtual int qt_metacall(QMetaObject::Call c, int id, void** arguments);
private:
    QMap<int, QMetaProperty> m_targetPropertyMap; //< QMap<signalId, QMetaProperty>
    QMap<QString, bool> m_propertyMap;
protected:
    LiteObject* m_binder;
    virtual void bindToTarget() override;
    virtual void copyProperty();

};
#endif