#include <QApplication>
#include <QDebug>

#include "Command/Command"
#include "DependencyObject/DependencyObject"
#include "Global/Global"

#include "mainwindow.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Logger::Install();
    MainWindow mainwindow;

    mainwindow.show();
    return app.exec();
}