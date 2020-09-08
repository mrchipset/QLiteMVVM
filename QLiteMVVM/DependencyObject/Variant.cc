#include "Variant.h"

ObserverableVector::ObserverableVector(const QString& name, LiteObject* parent) 
    : DataObject(name, parent), QVector<double>()
{

}