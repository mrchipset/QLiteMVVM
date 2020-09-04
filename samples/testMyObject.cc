// #include <QCoreApplication>
#include <QApplication>
#include <QDebug>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QtConcurrent>
#include <QJSEngine>

#include "Global/Global"
#include "DependencyObject/DependencyObject"
#include "DependencyObject/Property.h"
#include "Widget/Widget.h"
#include "Widget/TreeWidget.h"
#include "myObject.h"

void testObject(LiteObject* rootObject)
{
    MyObject myObject(rootObject);
    LiteObjectPropertyListener listener(&myObject, rootObject);
    qDebug() << QThread::currentThreadId();
    QtConcurrent::run(&myObject, &MyObject::setFoo, QString("bar-c"));
    myObject.setFoo("foo");
    myObject.setProperty("foo2", "bar");
    // qDebug() << "Start MyObject";
    FakeObjectListener::GetMetaSignal(&myObject);
    // qDebug() << "Start RootObject";
    FakeObjectListener::GetMetaSignal(rootObject);
    QJSEngine engine;
    QJSValue jsObject = engine.newQObject(&myObject);
    engine.globalObject().setProperty("myObject", jsObject);
    engine.evaluate("myObject.foo = \"hello\"");
}

void testProperty(LiteObject* rootObject)
{
    NumericProperty property("property->1", 0, 9, rootObject);
    qDebug() << property.setValue(10);
    QJSEngine engine;
    QJSValue jsObject = engine.newQObject(&property);
    engine.globalObject().setProperty("myProperty", jsObject);
    engine.evaluate("myProperty.upper = 4");
    engine.evaluate("myProperty.lower = 2");
    engine.evaluate("myProperty.value = 3.5");
    qDebug() << property.setValue(8);
    qDebug() << property.getValue();
    qDebug() << property.type();
}

void testWidget(LiteObject* rootObject)
{
    static QMainWindow window;
    QVBoxLayout * layout = new QVBoxLayout(&window);
    QPushButton *enable = new QPushButton("enable", &window);
    QPushButton *disable = new QPushButton("disable", &window);
    QPushButton *label = new QPushButton("Hello", &window);
    ObjectTreeWidget * tree = new ObjectTreeWidget(&window);
    layout->addWidget(tree);
    layout->addWidget(enable);
    layout->addWidget(disable);
    layout->addWidget(label);
    Widget *widget = new Widget("Tree", rootObject);
    widget->bindQWidget(label);
    QMap<QString, Property *> properties = widget->property("properties").value<QMap<QString, Property *>>();
    qDebug() << properties.size();
    widget->printProperties();
    properties["enabled"]->setValue(false);
    QObject::connect(enable, &QPushButton::clicked, [=](){
        QMap<QString, Property *> properties = widget->property("properties").value<QMap<QString, Property *>>();
        properties["enabled"]->setValue(true);
    });
    QObject::connect(disable, &QPushButton::clicked, [=](){
        QMap<QString, Property *> properties = widget->property("properties").value<QMap<QString, Property *>>();
            properties["enabled"]->setValue(false);
    });
    window.setCentralWidget(new QWidget(&window));
    window.centralWidget()->setLayout(layout);
    window.show();
    
    // properties.first()->setValue(100);
    // properties.last()->setValue(false);
}
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Logger::GetInstance();
    LiteObject* rootObject = &LiteObject::CreateRootObject();
    LiteObject* grp1 = new LiteObject("grp1", rootObject);
    LiteObject* grp2 = new LiteObject("grp2", rootObject);
    LiteObject* item1_1 = new LiteObject("item1", grp1);
    LiteObject* item2_1 = new LiteObject("item1", grp2);

    testWidget(rootObject);
    LiteObject* item1_2 = new LiteObject("item2", grp1);
    MyObject* item2_2 = new MyObject();
    // testObject(rootObject);
    // testProperty(rootObject);
    // return 0;
    return app.exec();
}

