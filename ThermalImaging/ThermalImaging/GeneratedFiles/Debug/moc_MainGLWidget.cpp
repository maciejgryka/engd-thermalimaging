/****************************************************************************
** Meta object code from reading C++ file 'MainGLWidget.h'
**
** Created: Fri 11. Jun 11:07:34 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MainGLWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainGLWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainGLWidget[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      24,   13,   13,   13, 0x0a,
      50,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainGLWidget[] = {
    "MainGLWidget\0\0clicked()\0"
    "thermalVisualPercent(int)\0executeRansac()\0"
};

const QMetaObject MainGLWidget::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_MainGLWidget,
      qt_meta_data_MainGLWidget, 0 }
};

const QMetaObject *MainGLWidget::metaObject() const
{
    return &staticMetaObject;
}

void *MainGLWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainGLWidget))
        return static_cast<void*>(const_cast< MainGLWidget*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int MainGLWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: clicked(); break;
        case 1: thermalVisualPercent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: executeRansac(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void MainGLWidget::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
