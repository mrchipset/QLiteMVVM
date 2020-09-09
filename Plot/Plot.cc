#include "Plot.h"

Figure::Figure(QWidget* parent) : QCustomPlot(parent)
{

}

Figure* Figure::CreatePlot(const QString& plotName, LiteObject* hostObject, QWidget* parent)
{
    Figure* plot = new Figure(parent);
    hostObject->setProperty(plotName.toLocal8Bit(), QVariant::fromValue<Figure*>(plot));
    plot->setObjectName(plotName);
    return plot;
}

void Figure::plot()
{
    this->replot(QCustomPlot::rpQueuedReplot);
}