#ifndef QLITEMVVM_PLOT_PLOT_H
#define QLITEMVVM_PLOT_PLOT_H

#include "qcustomplot.h"
#include "DependencyObject/Object.h"

class Figure : public QCustomPlot
{
    Q_OBJECT
    Q_PROPERTY(int graphCount READ graphCount)

public:
    Q_INVOKABLE Figure(QWidget* parent);
    Q_INVOKABLE static Figure* CreatePlot(const QString& plotName, LiteObject* hostObject, QWidget* parent);
    Q_INVOKABLE void plot();
};

#endif