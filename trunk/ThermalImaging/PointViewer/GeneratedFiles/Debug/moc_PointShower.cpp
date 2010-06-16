/****************************************************************************
** Meta object code from reading C++ file 'PointShower.h'
**
** Created: Wed 16. Jun 18:22:10 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../PointShower.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PointShower.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PointShower[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      23,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PointShower[] = {
    "PointShower\0\0clicked()\0newPoints(QString)\0"
};

const QMetaObject PointShower::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_PointShower,
      qt_meta_data_PointShower, 0 }
};

const QMetaObject *PointShower::metaObject() const
{
    return &staticMetaObject;
}

void *PointShower::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PointShower))
        return static_cast<void*>(const_cast< PointShower*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int PointShower::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: clicked(); break;
        case 1: newPoints((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void PointShower::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
