/****************************************************************************
** Meta object code from reading C++ file 'Interpreter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../GraphicEditor/Data/Interpreter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Interpreter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Interpreter_t {
    QByteArrayData data[15];
    char stringdata[165];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_Interpreter_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_Interpreter_t qt_meta_stringdata_Interpreter = {
    {
QT_MOC_LITERAL(0, 0, 11),
QT_MOC_LITERAL(1, 12, 9),
QT_MOC_LITERAL(2, 22, 0),
QT_MOC_LITERAL(3, 23, 5),
QT_MOC_LITERAL(4, 29, 10),
QT_MOC_LITERAL(5, 40, 6),
QT_MOC_LITERAL(6, 47, 8),
QT_MOC_LITERAL(7, 56, 18),
QT_MOC_LITERAL(8, 75, 12),
QT_MOC_LITERAL(9, 88, 4),
QT_MOC_LITERAL(10, 93, 17),
QT_MOC_LITERAL(11, 111, 8),
QT_MOC_LITERAL(12, 120, 8),
QT_MOC_LITERAL(13, 129, 18),
QT_MOC_LITERAL(14, 148, 15)
    },
    "Interpreter\0emitError\0\0error\0emitOutput\0"
    "output\0workDone\0stopRunningWorkers\0"
    "makeUserCall\0call\0makeProtectedCall\0"
    "makeCall\0stopWork\0finishedLoadConfig\0"
    "errorLoadConfig\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Interpreter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x05,
       4,    1,   67,    2, 0x05,
       6,    0,   70,    2, 0x05,
       7,    0,   71,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       8,    1,   72,    2, 0x0a,
      10,    1,   75,    2, 0x0a,
      11,    1,   78,    2, 0x0a,
      12,    0,   81,    2, 0x0a,
      13,    0,   82,    2, 0x08,
      14,    1,   83,    2, 0x08,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,
    QMetaType::Void, QMetaType::QByteArray,    5,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray,    9,
    QMetaType::Void, QMetaType::QByteArray,    9,
    QMetaType::Void, QMetaType::QByteArray,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    3,

       0        // eod
};

void Interpreter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Interpreter *_t = static_cast<Interpreter *>(_o);
        switch (_id) {
        case 0: _t->emitError((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->emitOutput((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 2: _t->workDone(); break;
        case 3: _t->stopRunningWorkers(); break;
        case 4: _t->makeUserCall((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 5: _t->makeProtectedCall((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 6: _t->makeCall((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 7: _t->stopWork(); break;
        case 8: _t->finishedLoadConfig(); break;
        case 9: _t->errorLoadConfig((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Interpreter::*_t)(const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Interpreter::emitError)) {
                *result = 0;
            }
        }
        {
            typedef void (Interpreter::*_t)(const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Interpreter::emitOutput)) {
                *result = 1;
            }
        }
        {
            typedef void (Interpreter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Interpreter::workDone)) {
                *result = 2;
            }
        }
        {
            typedef void (Interpreter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Interpreter::stopRunningWorkers)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject Interpreter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Interpreter.data,
      qt_meta_data_Interpreter,  qt_static_metacall, 0, 0}
};


const QMetaObject *Interpreter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Interpreter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Interpreter.stringdata))
        return static_cast<void*>(const_cast< Interpreter*>(this));
    return QObject::qt_metacast(_clname);
}

int Interpreter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void Interpreter::emitError(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Interpreter::emitOutput(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Interpreter::workDone()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Interpreter::stopRunningWorkers()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
