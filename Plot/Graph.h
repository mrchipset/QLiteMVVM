#ifndef PLOT_GRAPH_H
#define PLOT_GRAPH_H

#include "qcustomplot.h"

class Graph : public QCPGraph
{
    Q_OBJECT
public:
    explicit Graph(QCPAxis *keyAxis, QCPAxis *valueAxis);
    Q_INVOKABLE void setData(const QVector<double>& key, const QVector<double>& value);
};

#endif