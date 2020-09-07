#include "Property.h"

#include <QCoreApplication>

#include <QWidget>
#include <QDebug>

#define IN_RANGE(val, lower, upper) lower <= val && val <= upper

#pragma region class Property

Property::Property(const QMetaProperty& metaProperty, QObject* object, LiteObject* parent)
    : LiteObject(metaProperty.name(), parent), m_bindObject(object),
    m_readable(metaProperty.isReadable()),
    m_writeable(metaProperty.isWritable()),
    m_bind(true)
{
    connect(this, &Property::valueChanged, &Property::propertyChanged);
}
Property::Property(const QString& propName, LiteObject* parent) 
    : LiteObject(propName, parent), m_readable(true), m_writeable(true),
    m_bind(false)
{
    connect(this, &Property::valueChanged, &Property::propertyChanged);
}

Property::~Property()
{

}

Property::PropertyType Property::type() const
{
    return Base;
}

bool Property::setValue(const QVariant& val)
{
    bool ret = checkValid(val);
    bool changed = false;
    if (ret)
    {

        if (m_bind)
        {
            if(m_bindObject->setProperty(name().toLocal8Bit(), val))
            {
                m_value = val;
                changed = true;
            }
        }
        else
        {
            m_value = val;
            changed = true;
        }
        
    }
    if (changed)
    {
        emit(valueChanged());
        QCoreApplication::postEvent(rootObject(), new LitePropertyChangedEvent(this, name()));
    }
    return ret && changed;
}

QVariant Property::getValue() const
{
    if (m_bind)
    {
        return m_bindObject->property(name().toLocal8Bit());
    }
    return m_value;
}

void Property::setReadable(bool b)
{
    m_readable = b;
}

bool Property::isReadable() const
{
    return m_readable;
}

void Property::setWriteable(bool b)
{
    m_writeable = b;
}

bool Property::isWriteable() const
{
    return m_writeable;
}

bool Property::checkValid(const QVariant& val)
{
    return m_writeable;
}
#pragma endregion

#pragma region class RangeProperty

RangeProperty::RangeProperty(const QString& propName, const QVariant& lower,
    const QVariant& upper, LiteObject* parent)  : Property(propName, parent),
    m_lower(lower), m_upper(upper)
{
    connect(this, &RangeProperty::upperChanged, &RangeProperty::valueChanged);
    connect(this, &RangeProperty::lowerChanged, &RangeProperty::valueChanged);
}


RangeProperty::RangeProperty(const QString& propName, LiteObject* parent)
    :   RangeProperty(propName, 0, 1, parent)
{

}

Property::PropertyType RangeProperty::type() const
{
    return Range;
}

bool RangeProperty::setUpper(const QVariant& val)
{
    if(IN_RANGE(val.type(), QVariant::Int, QVariant::Double))
    {
        m_upper = val;
        if(m_upper < m_lower)
        {
            qSwap(m_upper, m_lower);
        }
        emit(upperChanged());
        return true;
    }
    return false;
}

bool RangeProperty::setLower(const QVariant& val)
{
    if(IN_RANGE(val.type(), QVariant::Int, QVariant::Double))
    {
        m_lower = val;
        if(m_upper < m_lower)
        {
            qSwap(m_upper, m_lower);
        }
        emit(lowerChanged());
        return true;
    }
    return false;
}

QVariant RangeProperty::getUpper() const
{
    return m_upper;
}

QVariant RangeProperty::getLower() const
{
    return m_lower;
}

bool RangeProperty::checkValid(const QVariant& val)
{
    return IN_RANGE(val.type(), QVariant::Int, QVariant::Double);
}

#pragma endregion

#pragma region class NumericProperty

NumericProperty::NumericProperty(const QString& propName,
    const QVariant& lower, const QVariant& upper, LiteObject* parent) 
    : Property(propName, parent),
    m_lower(lower), m_upper(upper)
{
    m_value = (m_lower.toDouble() + m_upper.toDouble()) / 2;
}

NumericProperty::NumericProperty(const QString& propName, LiteObject* parent)
    : NumericProperty(propName, 0, 1, parent)
{
    
}

Property::PropertyType NumericProperty::type() const
{
    return Numeric;
}

bool NumericProperty::setUpper(const QVariant& val)
{
    if(IN_RANGE(val.type(), QVariant::Int, QVariant::Double))
    {
        m_upper = val;
        if(m_upper < m_lower)
        {
            qSwap(m_upper, m_lower);
        }
        emit(upperChanged());
        return true;
    }
    return false;
}

bool NumericProperty::setLower(const QVariant& val)
{
    if(IN_RANGE(val.type(), QVariant::Int, QVariant::Double))
    {
        m_lower = val;
        if(m_upper < m_lower)
        {
            qSwap(m_upper, m_lower);
        }
        emit(lowerChanged());
        return true;
    }
    return false;
}

QVariant NumericProperty::getUpper() const
{
    return m_upper;
}

QVariant NumericProperty::getLower() const
{
    return m_lower;
}


bool NumericProperty::checkValid(const QVariant& val)
{
    return IN_RANGE(val.type(), QVariant::Int, QVariant::Double)
        && IN_RANGE(val, m_lower, m_upper);
}
#pragma endregion

#pragma region  class BooleanProperty

BooleanProperty::BooleanProperty(const QString& propName, LiteObject* parent) 
    : Property(propName, parent)
{
    m_value = false;
}

Property::PropertyType BooleanProperty::type() const
{
    return Boolean;
}

bool BooleanProperty::setValue(bool b)
{
    m_value = b;
    return true;
}

QVariant BooleanProperty::getValue() const
{
    return m_value;
}

bool BooleanProperty::checkValid(const QVariant& val)
{
    return val.type() == QVariant::Bool;
}
#pragma endregion

#pragma region class PropertyFactory
PropertyFactory::PropertyFactory(LiteObject* owner) : LiteObject(owner),
    m_owner(owner)
{

}

Property* PropertyFactory::createProperty(const QMetaProperty& metaProperty, QObject * object)
{
    return new Property(metaProperty, object, m_owner);
}

Property* PropertyFactory::createProperty(const QString& name)
{
    return new Property(name, m_owner);
}

Property* PropertyFactory::CreateProperty(const QMetaProperty& metaProperty, QObject * object, LiteObject* owner)
{
    return new Property(metaProperty, object, owner);
}

Property* PropertyFactory::CreateProperty(const QString& name, LiteObject* owner)
{
    return new Property(name, owner);
}

#pragma endregion