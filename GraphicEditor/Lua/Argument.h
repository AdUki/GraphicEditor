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
    Argument(const void* pointer);

    void setNumber(int number);
    void setString(const QByteArray& string);
    void setPointer(const void* pointer);
    void pushToState(lua_State* L);

private:
    enum class State {
        Uninitialized = 0,
        Number,
        String,
        Pointer
    };

    QByteArray _string;
    int _number;
    const void* _pointer;

    State _state;
};
