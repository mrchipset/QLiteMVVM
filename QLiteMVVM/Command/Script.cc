#include "Script.h"
#include <QDebug>
#include <QCoreApplication>
ScriptEngine* ScriptEngine::engine = nullptr;

ScriptEngine::~ScriptEngine()
{

}

ScriptEngine* ScriptEngine::GetSingleton()
{
    if (engine == nullptr)
    {
        engine = new ScriptEngine();
    }
    return engine;
}

ScriptEngine::ScriptEngine() : QScriptEngine(QCoreApplication::instance())
{

}

bool ScriptEngine::registerObject(const QString& name, QObject* object, const QString& target)
{
    

    if(object==nullptr)
    {
        qWarning() << "The QObject Pointer is nullptr";
        return false;
    }

    if (name.isEmpty())
    {
        qWarning() << "The property name can't be empty";
        return false;
    }

    QScriptValue targetObject;
    if (target.isEmpty())
    {
        targetObject = globalObject();
    }else
    {
        targetObject = globalObject().property(target);
        if(!targetObject.isValid())
        {
            qWarning() << "The target property is not existed.";
            return false;
        }
    }

    QScriptValue jsObject = newQObject(object);
    if(targetObject.property(name).isValid())
    {
        qWarning() << "The property is existed. Please change a name.";
        return false;
    }
    targetObject.setProperty(name, jsObject);
    return true;
}