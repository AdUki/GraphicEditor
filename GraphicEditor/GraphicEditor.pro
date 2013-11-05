#-------------------------------------------------
#
# Project created by QtCreator 2013-08-30T17:03:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphicEditor
TEMPLATE = app

CONFIG   += \
    c++11 \
    console \

SOURCES  += \
    main.cpp\
    mainwindow.cpp \
    Ui/Console.cpp \
    Data/Interpreter.cpp \
    Lua/LuaBindings.cpp \
    Ui/Grids/VerticalGrid.cpp \
    Ui/Grids/HorizontalGrid.cpp \
    Ui/Grids/BaseGrid.cpp \
    Ui/Items/TextItem.cpp \
    Ui/Items/ImageItem.cpp \
    Ui/Items/BaseItem.cpp \
    Data/TextFile.cpp \
    Data/FileManager.cpp \
    Lua/LuaWorker.cpp \
    Lua/Argument.cpp \
    Ui/BaseElement.cpp \
    Lua/ElementAllocator.cpp \
    Lua/ElementDeleter.cpp \
    Lua/ElementUpdater.cpp \
    Data/ElementManager.cpp \
    Ui/Grids/LinearGrid.cpp

HEADERS  += \
    mainwindow.h \
    Ui/Console.h \
    Data/Interpreter.h \
    Lua/LuaBindings.h \
    Lua/LuaInitScript.lua \
    Ui/Grids/VerticalGrid.h \
    Ui/Grids/HorizontalGrid.h \
    Ui/Grids/BaseGrid.h \
    Ui/Items/TextItem.h \
    Ui/Items/ImageItem.h \
    Ui/Items/BaseItem.h \
    Data/TextFile.h \
    Data/FileManager.h \
    Lua/LuaWorker.h \
    Lua/Argument.h \
    Ui/BaseElement.h \
    Lua/ElementAllocator.h \
    Lua/ElementDeleter.h \
    Lua/ElementUpdater.h \
    Data/ElementManager.h \
    Ui/Grids/LinearGrid.h

FORMS    += \
    mainwindow.ui

LuaPath = ../LuaJIT-2.0.2/src
LPegPath = ../lpeg-0.12
LfsPath = ../luafilesystem/src

LIBS     += \
    -L$${LuaPath} -llua51

INCLUDEPATH += \
    D:\\boost_1_54_0 \
    $${LuaPath}

DEPENDPATH += Lua/LuaInitScript.lua

CONFIG(debug, debug|release) {
    sharedlibs.path = $${OUT_PWD}/debug/
    grammars.path = $${OUT_PWD}/Grammars
} else {
    sharedlibs.path = $${OUT_PWD}/release/
    grammars.path = $${OUT_PWD}/Grammars
}
sharedlibs.files   += \
    $${LuaPath}/lua51.dll \
    $${LPegPath}/lpeg.dll \
    $${LfsPath}/lfs.dll \

grammars.files = Grammars/*.lua

INSTALLS += sharedlibs \
            grammars
