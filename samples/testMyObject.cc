#include <QCoreApplication>
#include <QDebug>
#include <QtConcurrent>
#include <QJSEngine>

#include "Global/Global"
#include "DependencyObject/DependencyObject"
#include "myObject.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    Logger::GetInstance();
    LiteObject* rootObject = &LiteObject::CreateRootObject();
    MyObject myObject(rootObject);
    LiteObjectPropertyListener listener(&myObject, rootObject);
    qDebug() << QThread::currentThreadId();
    QtConcurrent::run(&myObject, &MyObject::setFoo, QString("bar-c"));
    myObject.setFoo("foo");
    myObject.setProperty("foo2", "bar");
    qDebug() << "Start MyObject";
    FakeObjectListener::GetMetaSignal(&myObject);
    qDebug() << "Start RootObject";
    FakeObjectListener::GetMetaSignal(rootObject);
    QJSEngine engine;
    QJSValue jsObject = engine.newQObject(&myObject);
    engine.globalObject().setProperty("myObject", jsObject);
    engine.evaluate("myObject.foo = \"hello\"");
    return app.exec();
}