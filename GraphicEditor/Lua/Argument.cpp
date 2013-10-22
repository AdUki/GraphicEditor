#include "Argument.h"

#include <lua.hpp>

////////////////////////////////////////////////////////////////
Argument::Argument()
    : _number(0)
    , _state(State::Uninitialized)
{

}

////////////////////////////////////////////////////////////////
Argument::Argument(int number)
    : _number(number)
    , _state(State::Number)
{

}

////////////////////////////////////////////////////////////////
Argument::Argument(const QByteArray& string)
    : _string(string)
    , _state(State::String)
{

}

////////////////////////////////////////////////////////////////
Argument::Argument(const QString& string)
    : _string(string.toLocal8Bit())
    , _state(State::String)
{

}

////////////////////////////////////////////////////////////////
void Argument::setNumber(int number) {
    _state = State::Number;
    _number = number;
}

////////////////////////////////////////////////////////////////
void Argument::setString(const QByteArray& string) {
    _state = State::String;
    _string = string;
}

////////////////////////////////////////////////////////////////
void Argument::pushToState(lua_State* L)
{
    switch (_state) {

    case State::String:
        lua_pushlstring(L, _string.data(), _string.size());
        break;

    case State::Number:
        lua_pushinteger(L, _number);
        break;

    case State::Uninitialized:
        Q_ASSERT(false);
        break;
    }
}
