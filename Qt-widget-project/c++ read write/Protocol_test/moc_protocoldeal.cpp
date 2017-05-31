/****************************************************************************
** Meta object code from reading C++ file 'protocoldeal.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "protocoldeal.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'protocoldeal.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Protocoldeal_t {
    QByteArrayData data[5];
    char stringdata[55];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Protocoldeal_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Protocoldeal_t qt_meta_stringdata_Protocoldeal = {
    {
QT_MOC_LITERAL(0, 0, 12), // "Protocoldeal"
QT_MOC_LITERAL(1, 13, 20), // "AcceptDataFormBottom"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 1), // "s"
QT_MOC_LITERAL(4, 37, 17) // "AcceptDataFormTop"

    },
    "Protocoldeal\0AcceptDataFormBottom\0\0s\0"
    "AcceptDataFormTop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Protocoldeal[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,
       4,    0,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

       0        // eod
};

void Protocoldeal::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Protocoldeal *_t = static_cast<Protocoldeal *>(_o);
        switch (_id) {
        case 0: _t->AcceptDataFormBottom((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->AcceptDataFormTop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Protocoldeal::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Protocoldeal::AcceptDataFormBottom)) {
                *result = 0;
            }
        }
        {
            typedef void (Protocoldeal::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Protocoldeal::AcceptDataFormTop)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject Protocoldeal::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_Protocoldeal.data,
      qt_meta_data_Protocoldeal,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Protocoldeal::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Protocoldeal::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Protocoldeal.stringdata))
        return static_cast<void*>(const_cast< Protocoldeal*>(this));
    return QThread::qt_metacast(_clname);
}

int Protocoldeal::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Protocoldeal::AcceptDataFormBottom(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Protocoldeal::AcceptDataFormTop()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
