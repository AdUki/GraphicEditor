#pragma once

#include <QObject>
#include <QVector>

#include "./Argument.h"

class lua_State;

////////////////////////////////////////////////////////////////
class LuaWorker : public QObject
{
    Q_OBJECT

public:
    typedef QVector<Argument> Arguments;

    explicit LuaWorker();
    ~LuaWorker();

    void setScript(const QByteArray& script);
    void setFunction(const QString& name);

    template<typename T> void addArgument(const T& arg) {
        _arguments.append(Argument(arg));
    }

    void startProtected();
    void start();

private slots:
    void protectedCall();
    void call();

signals:
    void started(QThread* thread);
    void finished();
    void stop();
    void failed(const QByteArray& error);

private:
    QThread* _thread;

    QByteArray _function;
    QByteArray _script;
    Arguments _arguments;

    void prepareLuaState(lua_State* L);
};
