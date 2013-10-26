#pragma once

#include <lua.hpp>

void createBindings(lua_State* L);

int openFile(lua_State* L);
int closeFile(lua_State* L);
int saveFile(lua_State* L);

int QT_addElement(lua_State* L);
int QT_updateElement(lua_State* L);
int QT_removeElement(lua_State* L);
int QT_commitElementChanges(lua_State* L);

int dump(lua_State* L);

#include <vector>
#include <string>
