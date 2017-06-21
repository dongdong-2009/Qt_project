/****************************************************************************
** Meta object code from reading C++ file 'haldevice.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "haldevice.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'haldevice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_haldevice_t {
    QByteArrayData data[11];
    char stringdata0[152];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_haldevice_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_haldevice_t qt_meta_stringdata_haldevice = {
    {
QT_MOC_LITERAL(0, 0, 9), // "haldevice"
QT_MOC_LITERAL(1, 10, 20), // "mountedStatusChanged"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 15), // "readyBrowserUsb"
QT_MOC_LITERAL(4, 48, 4), // "path"
QT_MOC_LITERAL(5, 53, 16), // "propertyModified"
QT_MOC_LITERAL(6, 70, 10), // "numChanges"
QT_MOC_LITERAL(7, 81, 19), // "QList<ChangeStruct>"
QT_MOC_LITERAL(8, 101, 7), // "changes"
QT_MOC_LITERAL(9, 109, 23), // "proMountedStatusChanged"
QT_MOC_LITERAL(10, 133, 18) // "proReadyBrowserUsb"

    },
    "haldevice\0mountedStatusChanged\0\0"
    "readyBrowserUsb\0path\0propertyModified\0"
    "numChanges\0QList<ChangeStruct>\0changes\0"
    "proMountedStatusChanged\0proReadyBrowserUsb"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_haldevice[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       3,    1,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    2,   45,    2, 0x0a /* Public */,
       9,    1,   50,    2, 0x0a /* Public */,
      10,    1,   53,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::QString,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 7,    6,    8,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::QString,    4,

       0        // eod
};

void haldevice::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        haldevice *_t = static_cast<haldevice *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->mountedStatusChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->readyBrowserUsb((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->propertyModified((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QList<ChangeStruct>(*)>(_a[2]))); break;
        case 3: _t->proMountedStatusChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->proReadyBrowserUsb((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (haldevice::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&haldevice::mountedStatusChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (haldevice::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&haldevice::readyBrowserUsb)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject haldevice::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_haldevice.data,
      qt_meta_data_haldevice,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *haldevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *haldevice::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_haldevice.stringdata0))
        return static_cast<void*>(const_cast< haldevice*>(this));
    return QObject::qt_metacast(_clname);
}

int haldevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void haldevice::mountedStatusChanged(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void haldevice::readyBrowserUsb(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
