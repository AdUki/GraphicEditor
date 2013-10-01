/****************************************************************************
** Meta object code from reading C++ file 'Console.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../GraphicEditor/Ui/Console.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Console.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Console_t {
    QByteArrayData data[16];
    char stringdata[125];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_Console_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_Console_t qt_meta_stringdata_Console = {
    {
QT_MOC_LITERAL(0, 0, 7),
QT_MOC_LITERAL(1, 8, 9),
QT_MOC_LITERAL(2, 18, 0),
QT_MOC_LITERAL(3, 19, 5),
QT_MOC_LITERAL(4, 25, 10),
QT_MOC_LITERAL(5, 36, 6),
QT_MOC_LITERAL(6, 43, 9),
QT_MOC_LITERAL(7, 53, 5),
QT_MOC_LITERAL(8, 59, 4),
QT_MOC_LITERAL(9, 64, 5),
QT_MOC_LITERAL(10, 70, 5),
QT_MOC_LITERAL(11, 76, 11),
QT_MOC_LITERAL(12, 88, 4),
QT_MOC_LITERAL(13, 93, 10),
QT_MOC_LITERAL(14, 104, 11),
QT_MOC_LITERAL(15, 116, 7)
    },
    "Console\0emitInput\0\0input\0emitOutput\0"
    "output\0emitError\0error\0open\0close\0"
    "clear\0writeOutput\0data\0writeError\0"
    "writeSystem\0message\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Console[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x05,
       4,    1,   62,    2, 0x05,
       6,    1,   65,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       8,    0,   68,    2, 0x0a,
       9,    0,   69,    2, 0x0a,
      10,    0,   70,    2, 0x0a,
      11,    1,   71,    2, 0x0a,
      13,    1,   74,    2, 0x0a,
      14,    1,   77,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,
    QMetaType::Void, QMetaType::QByteArray,    5,
    QMetaType::Void, QMetaType::QByteArray,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,   12,
    QMetaType::Void, QMetaType::QByteArray,   12,
    QMetaType::Void, QMetaType::QString,   15,

       0        // eod
};

void Console::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Console *_t = static_cast<Console *>(_o);
        switch (_id) {
        case 0: _t->emitInput((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->emitOutput((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 2: _t->emitError((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 3: _t->open(); break;
        case 4: _t->close(); break;
        case 5: _t->clear(); break;
        case 6: _t->writeOutput((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 7: _t->writeError((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 8: _t->writeSystem((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Console::*_t)(const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Console::emitInput)) {
                *result = 0;
            }
        }
        {
            typedef void (Console::*_t)(const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Console::emitOutput)) {
                *result = 1;
            }
        }
        {
            typedef void (Console::*_t)(const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Console::emitError)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject Console::staticMetaObject = {
    { &QTextEdit::staticMetaObject, qt_meta_stringdata_Console.data,
      qt_meta_data_Console,  qt_static_metacall, 0, 0}
};


const QMetaObject *Console::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Console::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Console.stringdata))
        return static_cast<void*>(const_cast< Console*>(this));
    return QTextEdit::qt_metacast(_clname);
}

int Console::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void Console::emitInput(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Console::emitOutput(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Console::emitError(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
