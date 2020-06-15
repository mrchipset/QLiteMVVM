#include <QCoreApplication>
#include "DependencyObject/DependencyObject"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    LiteObject *object = LiteObject::CreateRootObject();
    return app.exec();
}