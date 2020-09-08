#ifndef PLOT_AXIS_H
#define PLOT_AXIS_H

#include "qcustomplot.h"

class Axis : public QCPAxis
{
    Q_OBJECT
public:
    explicit Axis(QCPAxisRect *parent, AxisType type);
    
};
#endif