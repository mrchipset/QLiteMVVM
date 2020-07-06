#include "myObject.h"
#include <QCoreApplication>
#include <QDebug>
#include <QtConcurrent>

MyObject::MyObject(LiteObject* parent) : LiteObject("foo", parent)
{
    connect(this, &MyObject::fooChanged, [ = ]() {
        qDebug() << "fooChanged" << m_foo;
    });
}

void MyObject::setFoo(QString bar)
{
    // qDebug() << QThread::currentThreadId();
    m_foo = bar;
    emit(fooChanged());
}

QString MyObject::foo() const
{
    return m_foo;
}

