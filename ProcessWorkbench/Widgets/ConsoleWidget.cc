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
    setCurrentRow(count() - 1);
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

CommandLineEdit::CommandLineEdit(QWidget* parent) : QLineEdit(parent),
    m_historyPosition(-1)
{

}
void CommandLineEdit::keyPressEvent(QKeyEvent* ev)
{
    switch (ev->key())
    {
    case Qt::Key_Up:
        if (m_historyPosition < m_historyCommands.size() - 1)
        {
            m_historyPosition++;
            setText(m_historyCommands.at(m_historyPosition));
        }
        ev->accept();
        break;
    case Qt::Key_Down:
        if (m_historyPosition > 0)
        {
            m_historyPosition--;
            setText(m_historyCommands.at(m_historyPosition));
        }
        ev->accept();
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        m_historyPosition = -1;
        if (m_historyCommands.size() + 1 > LimitedMaxItems)
        {
            m_historyCommands.removeLast();
        }
        m_historyCommands.insert(0, text());
    default:
        QLineEdit::keyPressEvent(ev);
        break;
    }

}

ConsoleWidget::ConsoleWidget(QWidget* parent) : QWidget(parent)
{
    m_pLayout = new QVBoxLayout(this);
    m_listWidget = new ConsoleListWidget(this);
    m_commandLine = new CommandLineEdit(this);
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
    m_commandLine->setFocus(Qt::OtherFocusReason);
}

