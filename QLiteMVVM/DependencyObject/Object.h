#ifndef QLITEMVVM_OBJECT_H
#define QLITEMVVM_OBJECT_H
#include <QEvent>
#include <QObject>
#include <QString>
#include <QMap>

#include "ObjectListener.h"
class LiteObject;

enum LiteEvent {
    LitePropertyChanged = QEvent::User + 1
};

class LitePropertyChangedEvent : public QEvent
{
public:
    LitePropertyChangedEvent(LiteObject* sender, const QString& propertyName);
    virtual ~LitePropertyChangedEvent();
    QString propertyName();
    LiteObject* sender();
    static const Type eventType;
private:
    LiteObject* m_sender;
    QString m_propertyName;
};

class LiteObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString objName READ name)
public:
    LiteObject(LiteObject* parent = nullptr);
    LiteObject(const QString& objName, LiteObject* parent = nullptr);
    virtual ~LiteObject();
    Q_INVOKABLE QString name() const;
    Q_INVOKABLE LiteObject* rootObject() const;
    Q_INVOKABLE LiteObject* parentObject() const;
    Q_INVOKABLE QString printChildren();
    Q_INVOKABLE LiteObject* getChildren(const QString& name);
    static LiteObject& CreateRootObject();
Q_SIGNALS:
    void objectTreeUpdated();
private:
    QString m_ObjectName;
    LiteObject* m_parentObject;
    static LiteObject RootObject;
protected:
    virtual void litePropertyChangedEvent(LitePropertyChangedEvent* ev);
    virtual bool event(QEvent* ev) override;
};

#endif