#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class lua_State;

extern bool _TERMINATE_LUA_THREAD;

class LuaWorker : public QObject
{
    Q_OBJECT

public:
    explicit LuaWorker(const QByteArray& code);
    ~LuaWorker();

    void startProtected();
    void start();

    void setTerminateEnabled() { _terminateEnabled = true; }

private slots:
    void protectedCall();
    void call();

signals:
    void started(QThread* thread);
    void finished();
    void stop();
    void failed(const QByteArray& error);

private:
    QByteArray _code;
    QThread* _thread;
    bool _terminateEnabled;
};

#endif // WORKER_H
