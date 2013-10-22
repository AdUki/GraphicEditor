#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QObject>

class lua_State;
class QMutex;
class TextFile;
class LuaWorker;

class Interpreter : public QObject
{
    Q_OBJECT

    explicit Interpreter(QObject *parent = 0);

public:
    ~Interpreter() override;

    static Interpreter* getInstance();

    lua_State* getState() { return _internalState; }
    QMutex* getWorkerMutex() { return _workerMutex; }

signals:
    void emitError(const QByteArray& error);
    void emitOutput(const QByteArray& output);
    void workDone();
    void stopRunningWorkers();

public slots:
    ////////////////////////////////////////////////////////////////
    void makeRegisterFileCall(const QString& file);
    void makeUnregisterFileCall(const QString& file);
    void makeReparseFileCall(const QString& file, const QString& text);
    void makeSetFileAbsolutePathCall(const QString& file, const QString& name);
    void makeSetFileGrammarCall(const QString& file, const QString& grammar);

    ////////////////////////////////////////////////////////////////
    void makeProtectedCall(const QByteArray& call);
    void makeCall(const QByteArray& call);
    void stopWork();

private slots:
    void finishedLoadConfig();
    void errorLoadConfig(const QByteArray& error);

private:
    lua_State* _internalState;
    QMutex* _workerMutex;

    void loadInitScript();
};

#endif // INTERPRETER_H
