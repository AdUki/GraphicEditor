#include "LuaWorker.h"

#include <lua.hpp>

#include <QMutex>
#include <QThread>
#include <QDebug>

#include "Data/Interpreter.h"

////////////////////////////////////////////////////////////////
LuaWorker::LuaWorker(const QByteArray& code)
: QObject()
, _code(code)
{
    _thread = new QThread();
    moveToThread(_thread);

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

    int error = luaL_loadbuffer(L, _code.data(), _code.length(), "pcall") ||
                lua_pcall(L, 0, 0, 0);

    Interpreter::getInstance()->getWorkerMutex()->unlock();

    if (error) {
        QByteArray message(lua_tostring(L, -1));
        lua_pop(L, 1);  /* pop error message from the stack */

        if (!message.endsWith('\n'))
            message.append('\n');

        emit failed(message);
    }

    emit finished();
}

////////////////////////////////////////////////////////////////
void LuaWorker::call()
{
    lua_State* L = Interpreter::getInstance()->getState();

    Interpreter::getInstance()->getWorkerMutex()->lock();

    emit started(_thread);

    luaL_loadbuffer(L, _code.data(), _code.length(), "call");
    lua_call(L, 0, 0);

    Interpreter::getInstance()->getWorkerMutex()->unlock();

    emit finished();
}
