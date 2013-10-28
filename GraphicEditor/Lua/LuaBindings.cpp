#include "LuaBindings.h"

#include <QObject>
#include <QDebug>

#include <boost/icl/interval_map.hpp>

#include "Ui/Items/BaseItem.h"
#include "Ui/Items/TextItem.h"
#include "Ui/Grids/BaseGrid.h"
#include "Ui/Grids/HorizontalGrid.h"

#include "Data/Interpreter.h"

////////////////////////////////////////////////////////////////
#define BIND_C_FUNCTION(name) \
    lua_pushcfunction(L, name); \
    lua_setglobal(L, #name);

////////////////////////////////////////////////////////////////
void createBindings(lua_State* L)
{
    BIND_C_FUNCTION(QT_addGrid);
    BIND_C_FUNCTION(QT_addItem);
    BIND_C_FUNCTION(QT_updateItem);
    BIND_C_FUNCTION(QT_removeElement);

    BIND_C_FUNCTION(openFile);
    BIND_C_FUNCTION(closeFile);
    BIND_C_FUNCTION(saveFile);

    BIND_C_FUNCTION(dump);
}

////////////////////////////////////////////////////////////////
struct ElementAllocator
{
    int startIndex;
    int endIndex;

    // TODO: formatovanie elementov
    std::string format;

    BaseItem** item;
    BaseGrid** grid;
};

QList<ElementAllocator> elementsToAdd;
QList<ElementAllocator> elementsToRemove;
QList<ElementAllocator> elementsToUpdate;

typedef boost::icl::left_open_interval<int> Interval;
typedef boost::icl::interval_map<Interval, BaseItem*> ItemsTree;
typedef boost::icl::interval_map<Interval, BaseGrid*> GridsTree;

////////////////////////////////////////////////////////////////
/// Parameters from Lua:
/// (QObject**) pointer to pointer of parent, is null if parent is root
/// (int) index from parent from new tree
/// returns pointer to pointer of created object
int QT_addItem(lua_State* L)
{
    BaseItem** parent = static_cast<BaseItem**>(lua_touserdata(L, 1));

    // TODO: Spravit elementFactory, aby nam vyrabala elementy...
    //       Faktory vyraba elementy v novom threade, lebo interpreter v nom bezi
    //       Faktory moze reusovat zmazane elementy
    void* newElement = new (void*)();
    Q_ASSERT(newElement != nullptr);
    qDebug() << "Add item " << newElement;

    lua_pushlightuserdata(L, newElement);

    // TODO: testovaci kod, objekty sa alokuju az po QT_commitChagnes
    BaseItem** element = static_cast<BaseItem**>(newElement);
    *element = new TextItem("TEST");

    return 1;
}

////////////////////////////////////////////////////////////////
/// Parameters from Lua:
/// (QObject**) pointer to pointer of parent, is null if parent is root
/// (int) index from parent from new tree
/// returns pointer to pointer of created object
int QT_addGrid(lua_State* L)
{
    BaseGrid** parent = static_cast<BaseGrid**>(lua_touserdata(L, 1));

    // TODO: Spravit elementFactory, aby nam vyrabala elementy...
    //       Faktory vyraba elementy v novom threade, lebo interpreter v nom bezi
    //       Faktory moze reusovat zmazane elementy
    void* newElement = new (void*)();
    Q_ASSERT(newElement != nullptr);
    qDebug() << "Add grid " << newElement;

    lua_pushlightuserdata(L, newElement);

    // TODO: testovaci kod, objekty sa alokuju az po QT_commitChagnes
    BaseGrid** element = static_cast<BaseGrid**>(newElement);
    *element = new HorizontalGrid();

    return 1;
}

////////////////////////////////////////////////////////////////
/// Parameters from Lua:
/// (QObject**) pointer to pointer of parent, is null if parent is root
/// (char*) new text of element on which is element going to be updated
int QT_updateItem(lua_State* L)
{
    BaseItem** element = static_cast<BaseItem**>(lua_touserdata(L, 1));

    qDebug() << "Update element " << static_cast<void*>(element);

    return 0;
}

////////////////////////////////////////////////////////////////
/// Parameters from Lua:
/// (QObject**) pointer to pointer of existing object
int QT_removeElement(lua_State* L)
{
    BaseItem** element = static_cast<BaseItem**>(lua_touserdata(L, 1));

    qDebug() << "Remove element " << static_cast<void*>(element);

    // Remove created object
    delete static_cast<BaseItem*>(*element);

    // Remove allocated pointer
    delete element;

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
