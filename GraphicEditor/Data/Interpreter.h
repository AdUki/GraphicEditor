#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QObject>

class lua_State;
class QMutex;
class TextFile;

class Interpreter : public QObject
{
    Q_OBJECT

    explicit Interpreter(QObject *parent = 0);

public:
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
