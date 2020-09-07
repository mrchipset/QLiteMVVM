#include "Widget.h"

#include <QDebug>

Widget::Widget(const QString& name, LiteObject* parent) 
    : LiteObject(name, parent), m_bindQWidget(nullptr)
{

}

Widget::~Widget()
{
    
}

bool Widget::bindQWidget(QWidget* widget)
{
    if (widget != nullptr)
    {
        disconnectQWidgetSignal();
        m_bindQWidget = widget;
        connectQWidgetSignal();
        createProperties();
        return true;
    }
    return false;
}

QWidget* Widget::bindedQWidget() const
{
    return m_bindQWidget;
}

QMap<QString, Property*> Widget::getProperties() const
{
    return m_properties;
}

bool Widget::setProperty(const QString& name, const QVariant& value)
{
    if (m_properties.contains(name))
    {
        return m_properties[name]->setValue(value);
    }
    return false;
}

void Widget::printProperties()
{
    for(auto iter=m_properties.begin(); iter!=m_properties.end();++iter)
    {
        Property* property = iter.value();

        qDebug() << iter.key() << property->getValue() 
        << property->isReadable() << property->isWriteable();
    }
}

void Widget::connectQWidgetSignal()
{
    if (m_bindQWidget != nullptr)
    {
        connect(m_bindQWidget, &QObject::destroyed, this, &Widget::onQWidgetDestroy);
    }
}

void Widget::disconnectQWidgetSignal()
{
    if (m_bindQWidget != nullptr)
    {
        disconnect(m_bindQWidget, &QObject::destroyed, this, &Widget::onQWidgetDestroy);
    }
}

void Widget::createProperties()
{
    qDeleteAll(m_properties);
    QList<QMetaProperty> metaProperties = FakeObjectListener::GetMetaProperty(m_bindQWidget);
    for(auto iter=metaProperties.begin(); iter!=metaProperties.end(); iter++)
    {
        Property* property =  PropertyFactory::CreateProperty(*iter, m_bindQWidget, this);
        m_properties.insert(iter->name(), property);
        // connect(property, &Property::valueChanged, this, &Widget::onValueChanged);
    }
}

void Widget::onQWidgetDestroy()
{
    qDeleteAll(m_properties);
    m_bindQWidget = nullptr;
}

// void Widget::onValueChanged()
// {
//     Property* property = qobject_cast<Property *>(sender());
//     if (property)
//     {
//         m_bindQWidget->setProperty(property->name().toLocal8Bit(), property->getValue());
//     }
// }