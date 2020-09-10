#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_engine = ScriptEngine::GetSingleton();
    m_engine->registerObject("mainwindow", this);
    qScriptRegisterSequenceMetaType<QList<QCPGraph*>>(m_engine);
    qScriptRegisterSequenceMetaType<QList<QCPAxisRect*>>(m_engine);
    qScriptRegisterSequenceMetaType<QList<QCPAxis*>>(m_engine);
    qScriptRegisterSequenceMetaType<QVector<double>>(m_engine);
    connectSignals();
}

MainWindow::~MainWindow()
{

}

bool MainWindow::createSubPlotWindow(const QString& title)
{
    FigureSubWindow* window = new FigureSubWindow(title, this);

    if(m_engine->registerObject(title, window, "mainwindow"))
    {
        ui->mdiArea->addSubWindow(window);
        window->show();
        return true;
    } else
    {
        window->deleteLater();
    }
}

void MainWindow::connectSignals()
{
    connect(ui->commandLine, &QLineEdit::returnPressed, this, &MainWindow::onCommandReturnPressed);
}

void MainWindow::onCommandReturnPressed()
{
    
    qDebug() << m_engine->evaluate(ui->commandLine->text()).toString();
    ui->commandLine->setText("");
}