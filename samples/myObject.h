#ifndef MYOBJECT_H
#define MYOBJECT_H
#include "DependencyObject/DependencyObject"

class MyObject : public LiteObject
{
    Q_OBJECT
    Q_PROPERTY(QString foo READ foo WRITE setFoo NOTIFY fooChanged)
public:
    Q_INVOKABLE MyObject(LiteObject* parent = nullptr);
    Q_INVOKABLE void setFoo(QString bar);
    Q_INVOKABLE QString foo() const;
    Q_SIGNAL void fooChanged();
private:
    QString m_foo;
    FakeObjectListener m_listener;

};
#endif