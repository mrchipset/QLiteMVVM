#ifndef QLITEMVVM_WIDGET_H
#define QLITEMVVM_WIDGET_H

#include <QMap>
#include <QWidget>
#include "../DependencyObject/Object.h"
#include "../DependencyObject/Property.h"


class FakeWidget : public LiteObject
{
    Q_OBJECT
    Q_PROPERTY(QMap<QString, Property*> properties READ getProperties)
public:
    FakeWidget(const QString& name, LiteObject* parent=nullptr);
    ~FakeWidget();
    Q_INVOKABLE bool bindQWidget(QWidget * widget);
    Q_INVOKABLE QWidget* bindedQWidget() const;
    QMap<QString, Property*> getProperties() const;
    bool setProperty(const QString& name, const QVariant& value);
    void printProperties();
private:
    void disconnectQWidgetSignal();
    void connectQWidgetSignal();
    void createProperties();
    Q_SLOT void onQWidgetDestroy();
    // Q_SLOT void onValueChanged();
protected:
    QMap<QString, Property*> m_properties;
    QWidget* m_bindQWidget;
};

// TODO Use eventfilter to install the fakewidget to Qt Widget?
#endif