#include "ConsoleWidget.h"

static const int DefaultMaxItems = 10;
static const int LimitedMaxItems = 100;


ConsoleListWidget::ConsoleListWidget(QWidget* parent) : QListWidget(parent),
    m_maxItems(DefaultMaxItems)
{

}

void ConsoleListWidget::clear()
{
    QListWidget::clear();
}

void ConsoleListWidget::addConsoleItem(const QString& content)
{
    if (count() > m_maxItems)
    {
        auto widget = takeItem(0);
        removeItemWidget(widget);
        delete widget;
    }
    new QListWidgetItem(content, this);
}

bool ConsoleListWidget::setMaxItems(int max)
{
    if (max > 0 && max < LimitedMaxItems)
    {
        m_maxItems = max;
        return true;
    }
    return false;
}

int ConsoleListWidget::getMaxItems() const
{
    return m_maxItems;
}


ConsoleWidget::ConsoleWidget(QWidget* parent) : QWidget(parent)
{
    m_pLayout = new QVBoxLayout(this);
    m_listWidget = new ConsoleListWidget(this);
    m_commandLine = new QLineEdit(this);
    m_pLayout->addWidget(m_listWidget);
    m_pLayout->addWidget(m_commandLine);
    setLayout(m_pLayout);
    m_engine = ScriptEngine::GetSingleton();
    connect(m_commandLine, &QLineEdit::returnPressed,
        this, &ConsoleWidget::onReturnPressed);
}

void ConsoleWidget::onReturnPressed()
{
    QString command = m_commandLine->text();
    QString result = m_engine->evaluate(command).toString();
    m_listWidget->addConsoleItem(result);
    m_commandLine->setText(QString());
}