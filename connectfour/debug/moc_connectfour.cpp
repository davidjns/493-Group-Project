/****************************************************************************
** Meta object code from reading C++ file 'connectfour.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../connectfour.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'connectfour.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ConnectFour_t {
    QByteArrayData data[5];
    char stringdata[63];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ConnectFour_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ConnectFour_t qt_meta_stringdata_ConnectFour = {
    {
QT_MOC_LITERAL(0, 0, 11),
QT_MOC_LITERAL(1, 12, 14),
QT_MOC_LITERAL(2, 27, 0),
QT_MOC_LITERAL(3, 28, 23),
QT_MOC_LITERAL(4, 52, 9)
    },
    "ConnectFour\0square_clicked\0\0"
    "processPendingDatagrams\0host_game\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConnectFour[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x0a,
       3,    0,   32,    2, 0x0a,
       4,    0,   33,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ConnectFour::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ConnectFour *_t = static_cast<ConnectFour *>(_o);
        switch (_id) {
        case 0: _t->square_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->processPendingDatagrams(); break;
        case 2: _t->host_game(); break;
        default: ;
        }
    }
}

const QMetaObject ConnectFour::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ConnectFour.data,
      qt_meta_data_ConnectFour,  qt_static_metacall, 0, 0}
};


const QMetaObject *ConnectFour::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConnectFour::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ConnectFour.stringdata))
        return static_cast<void*>(const_cast< ConnectFour*>(this));
    return QWidget::qt_metacast(_clname);
}

int ConnectFour::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
