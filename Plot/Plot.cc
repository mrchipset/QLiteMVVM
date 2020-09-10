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

FigureSubWindow::FigureSubWindow(const QString& title, QWidget* parent) : QMdiSubWindow(parent),
m_figure(new Figure(this))
{
    setWindowTitle(title);
    setWidget(m_figure);
}

QVariant FigureSubWindow::figure()
{
    return QVariant::fromValue<Figure*>(m_figure);
}