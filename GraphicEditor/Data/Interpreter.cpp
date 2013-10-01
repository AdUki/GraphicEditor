#include "interpreter.h"

#include <QDebug>
#include <QMutex>
#include <QApplication>
#include <QMessageBox>
#include <QThread>

#include <lua.hpp>

#include "Lua/LuaInitScript.lua"
#include "Lua/LuaBindings.h"

#include "./LuaWorker.h"

////////////////////////////////////////////////////////////////
Interpreter::Interpreter(QObject *parent)
: QObject(parent)
, _internalState(lua_open())
, _workerMutex(new QMutex())
{
    TerminateMutex = new QMutex();

    luaL_openlibs(_internalState);
    createBindings(_internalState);

    LuaWorker* worker = new LuaWorker(luaInitScript);
    connect(worker, SIGNAL(failed(QByteArray)), this, SIGNAL(emitError(QByteArray)));
    connect(worker, SIGNAL(finished()), this, SLOT(finishedLoadConfig()));
    worker->startProtected();
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
    static Interpreter* instance = new Interpreter();
    return instance;
}

////////////////////////////////////////////////////////////////
void Interpreter::makeUserCall(const QByteArray& call)
{
    LuaWorker* worker = new LuaWorker(call);
    worker->setTerminateEnabled();

    connect(worker, SIGNAL(failed(QByteArray)), this, SIGNAL(emitError(QByteArray)));
    connect(worker, SIGNAL(finished()), this, SIGNAL(workDone()));

    worker->startProtected();
}

////////////////////////////////////////////////////////////////
void Interpreter::makeProtectedCall(const QByteArray& call)
{
    LuaWorker* worker = new LuaWorker(call);

    connect(worker, SIGNAL(failed(QByteArray)), this, SIGNAL(emitError(QByteArray)));
    connect(worker, SIGNAL(finished()), this, SIGNAL(workDone()));

    worker->startProtected();
}

////////////////////////////////////////////////////////////////
void Interpreter::makeCall(const QByteArray& call)
{
    LuaWorker* worker = new LuaWorker(call);

    connect(worker, SIGNAL(failed(QByteArray)), this, SIGNAL(emitError(QByteArray)));
    connect(worker, SIGNAL(finished()), this, SIGNAL(workDone()));

    worker->start();
}

////////////////////////////////////////////////////////////////
void Interpreter::stopWork()
{
    qDebug() << "Terminate message recieved!";

    TerminateMutex->lock();
    _TERMINATE_LUA_THREAD = true;
    TerminateMutex->unlock();
}

////////////////////////////////////////////////////////////////
void Interpreter::finishedLoadConfig()
{
    qDebug() << "Lua interpreter started.";
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
