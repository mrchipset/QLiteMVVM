#ifndef PWB_MAINWINDOW_H
#define PWB_MAINWINDOW_H

#include <QMainWindow>
#include "Command/Command"
#include "Plot.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
} // namespace Ui
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent=nullptr);
    ~MainWindow();
    Q_INVOKABLE bool createSubPlotWindow(const QString& title);
private:
    Ui::MainWindow *ui;
    ScriptEngine* m_engine;
    void connectSignals();
    Q_SLOT void onCommandReturnPressed();
};

#endif