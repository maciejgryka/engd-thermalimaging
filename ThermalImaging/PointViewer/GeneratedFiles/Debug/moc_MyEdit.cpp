/****************************************************************************
** Meta object code from reading C++ file 'MyEdit.h'
**
** Created: Wed 16. Jun 16:19:34 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MyEdit.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MyEdit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MyEdit[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      28,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MyEdit[] = {
    "MyEdit\0\0pushPoints(QString)\0buttonClicked()\0"
};

const QMetaObject MyEdit::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_MyEdit,
      qt_meta_data_MyEdit, 0 }
};

const QMetaObject *MyEdit::metaObject() const
{
    return &staticMetaObject;
}

void *MyEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MyEdit))
        return static_cast<void*>(const_cast< MyEdit*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int MyEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: pushPoints((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: buttonClicked(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void MyEdit::pushPoints(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
