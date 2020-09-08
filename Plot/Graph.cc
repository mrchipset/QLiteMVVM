#include "Graph.h"

Graph::Graph(QCPAxis *keyAxis, QCPAxis *valueAxis) : QCPGraph(keyAxis, valueAxis)
{

}

void Graph::setData(const QVector<double>& key, const QVector<double>& value)
{
    QCPGraph::setData(key, value);
}