/****************************************************************************
** Meta object code from reading C++ file 'usbdetect.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Protocol_test/usbdetect.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'usbdetect.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UsbDetect_t {
    QByteArrayData data[5];
    char stringdata0[49];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UsbDetect_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UsbDetect_t qt_meta_stringdata_UsbDetect = {
    {
QT_MOC_LITERAL(0, 0, 9), // "UsbDetect"
QT_MOC_LITERAL(1, 10, 15), // "slotDeviceAdded"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 3), // "udi"
QT_MOC_LITERAL(4, 31, 17) // "slotDeviceRemoved"

    },
    "UsbDetect\0slotDeviceAdded\0\0udi\0"
    "slotDeviceRemoved"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UsbDetect[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x0a /* Public */,
       4,    1,   27,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,

       0        // eod
};

void UsbDetect::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UsbDetect *_t = static_cast<UsbDetect *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotDeviceAdded((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->slotDeviceRemoved((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject UsbDetect::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_UsbDetect.data,
      qt_meta_data_UsbDetect,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UsbDetect::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UsbDetect::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UsbDetect.stringdata0))
        return static_cast<void*>(const_cast< UsbDetect*>(this));
    return QWidget::qt_metacast(_clname);
}

int UsbDetect::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
