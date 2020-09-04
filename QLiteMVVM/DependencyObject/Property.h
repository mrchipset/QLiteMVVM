#ifndef QLITEMVVM_PROPERTY_H
#define QLITEMVVM_PROPERTY_H


#include <QObject>
#include <QVariant>
#include <QString>

#include "Object.h"


/**
 * @brief The base class for property
 * 
 */
class Property : public LiteObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant value WRITE setValue READ getValue NOTIFY valueChanged)
public:
    enum PropertyType
    {
        Base = 0,
        Range = 1,
        Numeric = 2,
        Boolean = 3,
        LastProperty = 0xFFFF
    };
    Q_ENUMS(PropertyType)
public:
    Property(const QMetaProperty& metaProperty, QObject * object, LiteObject* parent=nullptr);
    Property(const QString& propName, LiteObject* parent=nullptr);
    virtual ~Property();
    virtual Q_INVOKABLE PropertyType type() const;
    virtual Q_INVOKABLE bool setValue(const QVariant& val);
    virtual Q_INVOKABLE QVariant getValue() const;
    virtual Q_INVOKABLE void setReadable(bool b=true);
    virtual Q_INVOKABLE bool isReadable() const;
    virtual Q_INVOKABLE void setWriteable(bool b=true);
    virtual Q_INVOKABLE bool isWriteable() const;
Q_SIGNALS:
    void valueChanged();
    void propertyChanged();
private:
    bool m_readable;
    bool m_writeable;
    bool m_bind;
    QObject * m_bindObject;
protected:
    QVariant m_value;
    virtual bool checkValid(const QVariant& val);
};

class RangeProperty : public Property
{
    Q_OBJECT
    Q_PROPERTY(QVariant upper WRITE setUpper READ getUpper NOTIFY upperChanged)
    Q_PROPERTY(QVariant lower WRITE setLower READ getLower NOTIFY lowerChanged)

public:
    RangeProperty(const QString& propName, LiteObject* parent=nullptr);
    RangeProperty(const QString& propName, const QVariant& lower, const QVariant& upper, LiteObject* parent=nullptr);
    virtual Q_INVOKABLE PropertyType type() const;

    virtual Q_INVOKABLE bool setUpper(const QVariant& val);
    virtual Q_INVOKABLE bool setLower(const QVariant& val);
    virtual Q_INVOKABLE QVariant getUpper() const;
    virtual Q_INVOKABLE QVariant getLower() const;

Q_SIGNALS:
    void upperChanged();
    void lowerChanged();
protected:
    QVariant m_lower;
    QVariant m_upper;
    virtual bool checkValid(const QVariant& val) override;
};

class NumericProperty : public Property
{
    Q_OBJECT
    Q_PROPERTY(QVariant upper WRITE setUpper READ getUpper NOTIFY upperChanged)
    Q_PROPERTY(QVariant lower WRITE setLower READ getLower NOTIFY lowerChanged)
public:
    NumericProperty(const QString& propName, LiteObject* parent=nullptr);
    NumericProperty(const QString& propName, const QVariant& lower, const QVariant& upper, LiteObject* parent=nullptr);

    // virtual ~NumericProperty();
    virtual Q_INVOKABLE PropertyType type() const override;

    virtual Q_INVOKABLE bool setUpper(const QVariant& val);
    virtual Q_INVOKABLE bool setLower(const QVariant& val);
    virtual Q_INVOKABLE QVariant getUpper() const;
    virtual Q_INVOKABLE QVariant getLower() const;
Q_SIGNALS:
    void upperChanged();
    void lowerChanged();
protected:
    QVariant m_lower;
    QVariant m_upper;
    virtual bool checkValid(const QVariant& val) override;
};

class BooleanProperty : public Property
{
    Q_OBJECT
public:
    BooleanProperty(const QString& propName, LiteObject* parent=nullptr);
    virtual Q_INVOKABLE PropertyType type() const;
    virtual Q_INVOKABLE bool setValue(bool b);
    virtual Q_INVOKABLE QVariant getValue() const;
protected:
    bool checkValid(const QVariant& val);
};


#endif