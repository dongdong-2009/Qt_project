/****************************************************************************
** Meta object code from reading C++ file 'mediaplayer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MediaPlayer/mediaplayer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mediaplayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MediaPlayer_t {
    QByteArrayData data[10];
    char stringdata[215];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MediaPlayer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MediaPlayer_t qt_meta_stringdata_MediaPlayer = {
    {
QT_MOC_LITERAL(0, 0, 11), // "MediaPlayer"
QT_MOC_LITERAL(1, 12, 31), // "on_pushButton_open_file_clicked"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 36), // "on_pushButton_play_and_pause_..."
QT_MOC_LITERAL(4, 82, 10), // "onTimerOut"
QT_MOC_LITERAL(5, 93, 23), // "slider_progress_clicked"
QT_MOC_LITERAL(6, 117, 21), // "slider_progress_moved"
QT_MOC_LITERAL(7, 139, 24), // "slider_progress_released"
QT_MOC_LITERAL(8, 164, 28), // "on_pushButton_volume_clicked"
QT_MOC_LITERAL(9, 193, 21) // "slider_volume_changed"

    },
    "MediaPlayer\0on_pushButton_open_file_clicked\0"
    "\0on_pushButton_play_and_pause_clicked\0"
    "onTimerOut\0slider_progress_clicked\0"
    "slider_progress_moved\0slider_progress_released\0"
    "on_pushButton_volume_clicked\0"
    "slider_volume_changed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MediaPlayer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    0,   59,    2, 0x08 /* Private */,
       8,    0,   60,    2, 0x08 /* Private */,
       9,    0,   61,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MediaPlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MediaPlayer *_t = static_cast<MediaPlayer *>(_o);
        switch (_id) {
        case 0: _t->on_pushButton_open_file_clicked(); break;
        case 1: _t->on_pushButton_play_and_pause_clicked(); break;
        case 2: _t->onTimerOut(); break;
        case 3: _t->slider_progress_clicked(); break;
        case 4: _t->slider_progress_moved(); break;
        case 5: _t->slider_progress_released(); break;
        case 6: _t->on_pushButton_volume_clicked(); break;
        case 7: _t->slider_volume_changed(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject MediaPlayer::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MediaPlayer.data,
      qt_meta_data_MediaPlayer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MediaPlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MediaPlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MediaPlayer.stringdata))
        return static_cast<void*>(const_cast< MediaPlayer*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MediaPlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
