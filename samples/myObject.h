#ifndef MYOBJECT_H
#define MYOBJECT_H
#include "DependencyObject/DependencyObject"

class MyObject : public LiteObject
{
    Q_OBJECT
    Q_PROPERTY(QString foo READ foo WRITE setFoo NOTIFY fooChanged)
public:
    MyObject(LiteObject * parent=nullptr);
    void setFoo(QString bar);
    QString foo() const;
    Q_SIGNAL void fooChanged();
private:
    QString m_foo;

};
#endif