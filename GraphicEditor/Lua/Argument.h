#pragma once

#include <QByteArray>
#include <QString>

class lua_State;

///
/// Use implicit constructors to initialize
/// e. g. Interpreter::getInstance()->addArgument(8)
///       Interpreter::getInstance()->addArgument("value=8")
///
class Argument
{
public:
    Argument();
    Argument(int number);
    Argument(const QByteArray& string);
    Argument(const QString& string);

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
