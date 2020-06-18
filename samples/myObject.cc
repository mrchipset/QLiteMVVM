#include "myObject.h"
#include <QCoreApplication>
#include <QDebug>
#include <QtConcurrent>

MyObject::MyObject(LiteObject * parent) : LiteObject("foo", parent)
{
}

void MyObject::setFoo(QString bar)
{
    qDebug() << QThread::currentThreadId();
    m_foo = bar;
    emit(fooChanged());
    QCoreApplication::postEvent(this, new LitePropertyChangedEvent(this, "foo"));
}

QString MyObject::foo() const
{
    return m_foo;
}

