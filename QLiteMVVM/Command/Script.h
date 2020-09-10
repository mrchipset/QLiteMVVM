#ifndef QLITEMVVM_COMMAND_SCRIPT_H
#define QLITEMVVM_COMMAND_SCRIPT_H

#include <QScriptEngine>
#include <QString>

class ScriptEngine : public QScriptEngine
{
    Q_OBJECT
public:
    ~ScriptEngine();
    static ScriptEngine* GetSingleton();
    bool registerObject(const QString& name, QObject* object, const QString& target=QString());
private:
    static ScriptEngine* engine;

    ScriptEngine();
};

#endif