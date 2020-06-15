#include <QEvent>
#include <QObject>
#include <QString>

class LiteObject;

class LitePropertyChangedEvent : QEvent
{
public:
    LitePropertyChangedEvent(LiteObject * sender, const QString& propertyName);
    static const Type eventType;
private:
    LiteObject * m_sender;
    QString m_propertyName;
};

class LiteObject : QObject
{
    Q_OBJECT
    Q_PROPERTY(QString objName READ name)
public:
    LiteObject(const QString& objName, LiteObject * parent=nullptr);
    virtual ~LiteObject();
    Q_INVOKABLE QString name() const;
    Q_INVOKABLE LiteObject* rootObject() const;
    Q_INVOKABLE LiteObject* parentObject() const;
    static LiteObject * CreateRootObject();
private:
    QString m_ObjectName;
    LiteObject * m_parentObject;
    LiteObject * m_rootObject;
protected:
    virtual bool event(QEvent * ev) override;
    
};