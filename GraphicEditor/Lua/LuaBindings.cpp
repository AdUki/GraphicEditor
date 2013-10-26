#include "LuaBindings.h"

#include <QObject>
#include <QDebug>

#include "Data/Interpreter.h"

////////////////////////////////////////////////////////////////
#define BIND_C_FUNCTION(name) \
    lua_pushcfunction(L, name); \
    lua_setglobal(L, #name);

////////////////////////////////////////////////////////////////
void createBindings(lua_State* L)
{
    BIND_C_FUNCTION(QT_addElement);
    BIND_C_FUNCTION(QT_updateElement);
    BIND_C_FUNCTION(QT_removeElement);
    BIND_C_FUNCTION(QT_commitElementChanges);

    BIND_C_FUNCTION(openFile);
    BIND_C_FUNCTION(closeFile);
    BIND_C_FUNCTION(saveFile);

    BIND_C_FUNCTION(dump);
}

////////////////////////////////////////////////////////////////
QList<QObject*> elementsToAdd;
QList<QObject*> elementsToRemove;

////////////////////////////////////////////////////////////////
int QT_addElement(lua_State* L)
{
    lua_tostring(L, 1);


    qDebug() << "Add element";

    // TODO: Spravit elementFactory, aby nam vyrabala elementy...
    //       Faktory vyraba elementy v novom threade, lebo interpreter v nom bezi
    //       Faktory moze reusovat zmazane elementy
    QObject* newElement = new QObject();

    lua_pushlightuserdata(L, static_cast<void*>(newElement));

    return 1;
}

////////////////////////////////////////////////////////////////
int QT_updateElement(lua_State* L)
{
    Q_UNUSED(L);

    //TODO: implementacia

    qDebug() << "Update element";

    return 0;
}

////////////////////////////////////////////////////////////////
int QT_removeElement(lua_State* L)
{
    Q_UNUSED(L);

    //TODO: implementacia

    qDebug() << "Remove element";

    return 0;
}

////////////////////////////////////////////////////////////////
int QT_commitElementChanges(lua_State* L)
{
    Q_UNUSED(L);

    //TODO: implementacia

    qDebug() << "Committing element changes";

    return 0;
}

////////////////////////////////////////////////////////////////
int openFile(lua_State* L)
{
    Q_UNUSED(L);

    //TODO: implementacia

    return 0;
}

////////////////////////////////////////////////////////////////
int closeFile(lua_State* L)
{
    Q_UNUSED(L);

    //TODO: implementacia

    return 0;
}

////////////////////////////////////////////////////////////////
int saveFile(lua_State* L)
{
    Q_UNUSED(L);

    //TODO: implementacia

    return 0;
}

////////////////////////////////////////////////////////////////
int dump(lua_State* L)
{
    Interpreter* interpreter = Interpreter::getInstance();

    int top = lua_gettop(L);
    for (int i = 1; i <= top; i++) {  /* repeat for each level */

        if (i != 1)
            interpreter->emitOutput(QByteArray("\t")); /* put a separator */

        int t = lua_type(L, i);
        switch (t) {
            case LUA_TSTRING:  /* strings */
                emit interpreter->emitOutput(QByteArray(lua_tostring(L, i)));
                break;

            case LUA_TBOOLEAN:  /* booleans */
                emit interpreter->emitOutput(QByteArray(lua_toboolean(L, i) ? "true" : "false"));
                break;

            case LUA_TNUMBER:  /* numbers */
                emit interpreter->emitOutput(QByteArray::number(lua_tonumber(L, i)));
                break;

            default:  /* other values */
                emit interpreter->emitOutput(QByteArray(lua_typename(L, t)));
                break;

        }
    }
//    emit interpreter->emitOutput(QByteArray("\n")); /* end the listing */
    return 0;
}
