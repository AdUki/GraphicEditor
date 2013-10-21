#pragma once

#include <QObject>

class lua_State;

class LuaWorker : public QObject
{
    Q_OBJECT

public:
    explicit LuaWorker(const QByteArray& code);
    ~LuaWorker();

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
    QByteArray _code;
    QThread* _thread;
};
