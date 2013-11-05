#include "LuaWorker.h"

#include <lua.hpp>

#include <QMutex>
#include <QThread>
#include <QDebug>

#include "Data/Interpreter.h"

////////////////////////////////////////////////////////////////
LuaWorker::LuaWorker()
: QObject()
{
    _thread = new QThread();
    moveToThread(_thread);

    connect(this, SIGNAL(failed(QByteArray)), Interpreter::getInstance(), SIGNAL(emitError(QByteArray)));
    connect(this, SIGNAL(finished()), Interpreter::getInstance(), SIGNAL(workDone()));
    connect(this, SIGNAL(finished()), _thread, SLOT(quit()));
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    connect(_thread, SIGNAL(finished()), _thread, SLOT(deleteLater()));

// NOTE expecting that all created workers will be started with start or startProtected
#ifdef QT_DEBUG
    qDebug() << "STARTED " << _thread;
#endif
}

////////////////////////////////////////////////////////////////
LuaWorker::~LuaWorker()
{
#ifdef QT_DEBUG
    qDebug() << "FINISHED" << _thread;
#endif
}

////////////////////////////////////////////////////////////////
void LuaWorker::setScript(const QByteArray& script)
{
    Q_ASSERT(_function.isEmpty());
    _script = script;
}

////////////////////////////////////////////////////////////////
void LuaWorker::setFunction(const QString& name)
{
    Q_ASSERT(_script.isEmpty());
    _function = name.toLocal8Bit();
}

////////////////////////////////////////////////////////////////
void LuaWorker::start()
{
    connect(_thread, SIGNAL(started()), this, SLOT(call()));
    _thread->start();
}

////////////////////////////////////////////////////////////////
void LuaWorker::startProtected()
{
    connect(_thread, SIGNAL(started()), this, SLOT(protectedCall()));
    _thread->start();
}

////////////////////////////////////////////////////////////////
void LuaWorker::protectedCall()
{
    lua_State* L = Interpreter::getInstance()->getState();

    Interpreter::getInstance()->getWorkerMutex()->lock();

    emit started(_thread);

    try {
        prepareLuaState(L);
        if (lua_pcall(L, _arguments.size(), 0, 0))
            throw 2;
    }
    catch(...) {

        QByteArray message(lua_tostring(L, -1));
        lua_pop(L, 1);  /* pop error message from the stack */

        emit failed(message);
    }

    Interpreter::getInstance()->getWorkerMutex()->unlock();

    emit finished();
}

////////////////////////////////////////////////////////////////
void LuaWorker::call()
{
    lua_State* L = Interpreter::getInstance()->getState();

    Interpreter::getInstance()->getWorkerMutex()->lock();

    emit started(_thread);

    prepareLuaState(L);
    lua_call(L, _arguments.size(), 0);

    Interpreter::getInstance()->getWorkerMutex()->unlock();

    emit finished();
}

////////////////////////////////////////////////////////////////
void LuaWorker::prepareLuaState(lua_State* L)
{
    // Push script
    if (!_script.isEmpty() &&
        luaL_loadbuffer(L, _script.data(), _script.length(), "script"))
        throw 1;

    // Push function and arguments
    if (!_function.isEmpty()) {

        lua_getglobal(L, _function);

        for (Argument& arg : _arguments)
            arg.pushToState(L);
    }
}
