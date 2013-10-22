#pragma once

#include <QObject>
#include <QVector>

class lua_State;

////////////////////////////////////////////////////////////////
class LuaWorker : public QObject
{
    Q_OBJECT

public:
    ////////////////////////////////////////////////////////////////
    // TODO: create own class
    /// Use implicit constructors to initialize
    /// e. g. Interpreter::getInstance()->addArgument(8)
    ///       Interpreter::getInstance()->addArgument("value=8")
    struct Argument {
        Argument() : _number(0), _state(State::Uninitialized) {}
        Argument(int number) : _number(number), _state(State::Number) { }
        Argument(const QByteArray& string) : _string(string), _state(State::String) { }
        Argument(const QString& string) : _string(string.toLocal8Bit()), _state(State::String) { }

        void setNumber(int number);
        void setString(const QByteArray& string);
        void pushToState(lua_State* L);

    private:
        enum class State {
            Uninitialized = 0,
            Number,
            String
        };

        QByteArray _string;
        int _number;
        State _state;
    };

    ////////////////////////////////////////////////////////////////
    explicit LuaWorker();
    ~LuaWorker();

    typedef QVector<Argument> Arguments;

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
