/****************************************************************************
** Meta object code from reading C++ file 'lua_worker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../GraphicEditor/Data/lua_worker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lua_worker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_LuaWorker_t {
    QByteArrayData data[7];
    char stringdata[53];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_LuaWorker_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_LuaWorker_t qt_meta_stringdata_LuaWorker = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 8),
QT_MOC_LITERAL(2, 19, 0),
QT_MOC_LITERAL(3, 20, 6),
QT_MOC_LITERAL(4, 27, 5),
QT_MOC_LITERAL(5, 33, 13),
QT_MOC_LITERAL(6, 47, 4)
    },
    "LuaWorker\0finished\0\0failed\0error\0"
    "protectedCall\0call\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LuaWorker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x05,
       3,    1,   35,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       5,    0,   38,    2, 0x08,
       6,    0,   39,    2, 0x08,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void LuaWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LuaWorker *_t = static_cast<LuaWorker *>(_o);
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->failed((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 2: _t->protectedCall(); break;
        case 3: _t->call(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (LuaWorker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LuaWorker::finished)) {
                *result = 0;
            }
        }
        {
            typedef void (LuaWorker::*_t)(const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LuaWorker::failed)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject LuaWorker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_LuaWorker.data,
      qt_meta_data_LuaWorker,  qt_static_metacall, 0, 0}
};


const QMetaObject *LuaWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LuaWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LuaWorker.stringdata))
        return static_cast<void*>(const_cast< LuaWorker*>(this));
    return QObject::qt_metacast(_clname);
}

int LuaWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void LuaWorker::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void LuaWorker::failed(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
