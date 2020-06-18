#include <QCoreApplication>
#include <QDebug>
#include <QtConcurrent>
#include "DependencyObject/DependencyObject"
#include "myObject.h"
int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    LiteObject *rootObject = &LiteObject::CreateRootObject();
    MyObject myObject(rootObject);
    qDebug() << QThread::currentThreadId();
    QtConcurrent::run(&myObject, &MyObject::setFoo, QString("bar-c"));
    // myObject.setFoo("foo");
    myObject.setProperty("foo2", "bar");
    return app.exec();
}