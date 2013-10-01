#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QObject>

class QMutex;
class lua_State;

class Interpreter : public QObject
{
    Q_OBJECT
public:
    explicit Interpreter(QObject *parent = 0);
    ~Interpreter() override;

    static Interpreter* getInstance();

    lua_State* getState() { return _internalState; }
    QMutex* getWorkerMutex() { return _workerMutex; }

    QMutex* TerminateMutex;

signals:
    void emitError(const QByteArray& error);
    void emitOutput(const QByteArray& output);
    void workDone();
    void stopRunningWorkers();

public slots:
    void makeUserCall(const QByteArray& call);
    void makeProtectedCall(const QByteArray& call);
    void makeCall(const QByteArray& call);
    void stopWork();

private slots:
    void finishedLoadConfig();
    void errorLoadConfig(const QByteArray& error);

private:
    lua_State* _internalState;
    QMutex* _workerMutex;
};

#endif // INTERPRETER_H
