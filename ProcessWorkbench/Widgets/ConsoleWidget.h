#ifndef PWB_CONSOLEWIDGET_H
#define PWB_CONSOLEWIDGET_H

#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QWidget>
#
#include "Command/Command"

class ConsoleListWidget : public QListWidget
{
    Q_OBJECT
    Q_PROPERTY(int maxItems WRITE setMaxItems READ getMaxItems)
public:
    ConsoleListWidget(QWidget* parent);
    Q_INVOKABLE void clear();
    void addConsoleItem(const QString& content);
    bool setMaxItems(int max);
    int getMaxItems() const;
private:
    int m_maxItems;

};

//TODO add history function
class ConsoleWidget : public QWidget
{
    Q_OBJECT
public:
    ConsoleWidget(QWidget* parent);

private:
    ConsoleListWidget* m_listWidget;
    QLineEdit* m_commandLine;
    QVBoxLayout* m_pLayout;
    ScriptEngine* m_engine;

    Q_SLOT void onReturnPressed();
};

#endif