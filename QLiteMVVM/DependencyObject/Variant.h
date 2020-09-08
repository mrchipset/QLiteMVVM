#ifndef QLITEMVVM_VARIANT_H
#define QLITEMVVM_VARIANT_H

#include <QVariant>
#include <QVector>

#include "Object.h"
#include "Property.h"

class ObserverableVector : public DataObject, public QVector<double>
{
    Q_OBJECT
public:
    ObserverableVector(const QString& name, LiteObject* parent=nullptr);
    
};




#endif