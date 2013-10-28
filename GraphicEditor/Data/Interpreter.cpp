#include "interpreter.h"

#include <QDebug>
#include <QMutex>
#include <QApplication>
#include <QMessageBox>
#include <QThread>

#include <lua.hpp>

#include "Lua/LuaInitScript.lua"
#include "Lua/LuaBindings.h"
#include "Lua/LuaWorker.h"

////////////////////////////////////////////////////////////////
Interpreter::Interpreter(QObject *parent)
: QObject(parent)
, _internalState(lua_open())
, _workerMutex(new QMutex())
{
    luaL_openlibs(_internalState);
    createBindings(_internalState);

    qDebug() << "Lua interpreter started.\n";
}

////////////////////////////////////////////////////////////////
Interpreter::~Interpreter()
{
    lua_close(_internalState);
    qDebug() << "Lua interpreter ended.";
}

////////////////////////////////////////////////////////////////
Interpreter* Interpreter::getInstance()
{
    static Interpreter* INSTANCE = nullptr;
    if (INSTANCE == nullptr) {
        INSTANCE = new Interpreter();
        INSTANCE->loadInitScript();
    }
    return INSTANCE;
}

////////////////////////////////////////////////////////////////
void Interpreter::loadInitScript()
{
    LuaWorker* worker = new LuaWorker();

    connect(worker, SIGNAL(failed(QByteArray)), this, SLOT(errorLoadConfig(QByteArray)));
    connect(worker, SIGNAL(finished()), this, SLOT(finishedLoadConfig()));
    connect(worker, SIGNAL(finished()), this, SIGNAL(workDone()));

    worker->setScript(luaInitScript);
    worker->startProtected();
}

////////////////////////////////////////////////////////////////
void Interpreter::makeRegisterFileCall(const void *file)
{
    LuaWorker* worker = new LuaWorker();
    worker->setFunction("registerFile");
    worker->addArgument(file);
    worker->startProtected();
}

////////////////////////////////////////////////////////////////
void Interpreter::makeUnregisterFileCall(const void *file)
{
    LuaWorker* worker = new LuaWorker();
    worker->setFunction("unregisterFile");
    worker->addArgument(file);
    worker->startProtected();
}

////////////////////////////////////////////////////////////////
void Interpreter::makeReparseFileCall(const void *file, const QString& text)
{
    LuaWorker* worker = new LuaWorker();
    worker->setFunction("reparseFile");
    worker->addArgument(file);
    worker->addArgument(text);
    worker->startProtected();
}

////////////////////////////////////////////////////////////////
void Interpreter::makeSetFileAbsolutePathCall(const void *file, const QString& name)
{
    LuaWorker* worker = new LuaWorker();
    worker->setFunction("setFileAbsolutePath");
    worker->addArgument(file);
    worker->addArgument(name);
    worker->startProtected();
}

////////////////////////////////////////////////////////////////
void Interpreter::makeSetFileGrammarCall(const void *file, const QString& grammar)
{
    LuaWorker* worker = new LuaWorker();
    worker->setFunction("setFileGrammar");
    worker->addArgument(file);
    worker->addArgument(grammar);
    worker->startProtected();
}

////////////////////////////////////////////////////////////////
void Interpreter::makeProtectedCall(const QByteArray& call)
{
    LuaWorker* worker = new LuaWorker();
    worker->setScript(call);
    worker->startProtected();
}

////////////////////////////////////////////////////////////////
void Interpreter::makeCall(const QByteArray& call)
{
    LuaWorker* worker = new LuaWorker();
    worker->setScript(call);
    worker->start();
}

////////////////////////////////////////////////////////////////
void Interpreter::stopWork()
{
    qDebug() << "Terminate message recieved!";

    // TODO: terminate
}

////////////////////////////////////////////////////////////////
void Interpreter::finishedLoadConfig()
{
    qDebug() << "Successfully loaded config.\n";
}

////////////////////////////////////////////////////////////////
void Interpreter::errorLoadConfig(const QByteArray& error)
{
    QMessageBox msgBox;
    msgBox.setInformativeText(error);
    msgBox.setText("Error during loading Lua initialization script.");
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();

    QApplication::exit(-1);
}
