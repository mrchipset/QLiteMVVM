#include <QApplication>
#include <QDebug>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QtConcurrent>
#include <QJSEngine>
#include <QScriptEngine>
#include <QColor>

#include <QTextEdit>
#include <QLabel>
#include <QtQml>

#include "Global/Global"
#include "DependencyObject/DependencyObject"
#include "DependencyObject/Property.h"
#include "Widget/Widget.h"
#include "Widget/TreeWidget.h"
#include "Widget/PropertyTableView.h"

#include "Plot.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    Logger::GetInstance();
    LiteObject* rootObject = &LiteObject::CreateRootObject();
    QScriptEngine engine;
    qScriptRegisterSequenceMetaType<QList<QCPGraph*>>(&engine);
    qScriptRegisterSequenceMetaType<QList<QCPAxisRect*>>(&engine);
    qScriptRegisterSequenceMetaType<QList<QCPAxis*>>(&engine);
    qScriptRegisterSequenceMetaType<QVector<double>>(&engine);

    static QScriptValue jsObject = engine.newQObject(rootObject);
    engine.globalObject().setProperty("rootObject", jsObject);
    static QScriptValue jsMetaObject = engine.newQMetaObject(&LiteObject::staticMetaObject);
    engine.globalObject().setProperty("LiteObject", jsMetaObject);
    QMainWindow window;
    Figure* figure = Figure::CreatePlot("figure", rootObject, &window);
    QVBoxLayout* layout = new QVBoxLayout(&window);
    QTextEdit* text = new QTextEdit(&window);
    QPushButton* button = new QPushButton("Eval", &window);
    QLabel* label = new QLabel(&window);
    QObject::connect(button, &QPushButton::clicked, [&]()
    {
        QString str = text->toPlainText();
        QScriptValue jsval = engine.evaluate(str);
        label->setText(jsval.toString());
    });
    
    layout->addWidget(text, 2);
    layout->addWidget(button, 1);
    layout->addWidget(label, 1);
    layout->addWidget(figure, 3);
    window.setCentralWidget(new QWidget(&window));
    window.centralWidget()->setLayout(layout);
    window.resize(640, 480);
    window.show();
    
    return app.exec();
}