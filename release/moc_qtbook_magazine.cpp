/****************************************************************************
** Meta object code from reading C++ file 'qtbook_magazine.h'
**
** Created: Tue Jun 26 01:58:49 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include.d/qtbook_magazine.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtbook_magazine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_qtbook_magazine[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x09,
      26,   16,   16,   16, 0x09,
      38,   16,   16,   16, 0x09,
      50,   16,   16,   16, 0x09,
      62,   16,   16,   16, 0x09,
      75,   16,   16,   16, 0x09,
      91,   16,   16,   16, 0x09,
     109,   16,   16,   16, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_qtbook_magazine[] = {
    "qtbook_magazine\0\0slotGo()\0slotPrint()\0"
    "slotQuery()\0slotReset()\0slotCancel()\0"
    "slotShowUsers()\0slotSelectImage()\0"
    "slotPopulateCopiesEditor()\0"
};

const QMetaObject qtbook_magazine::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_qtbook_magazine,
      qt_meta_data_qtbook_magazine, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &qtbook_magazine::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *qtbook_magazine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *qtbook_magazine::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qtbook_magazine))
        return static_cast<void*>(const_cast< qtbook_magazine*>(this));
    if (!strcmp(_clname, "qtbook_item"))
        return static_cast< qtbook_item*>(const_cast< qtbook_magazine*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int qtbook_magazine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: slotGo(); break;
        case 1: slotPrint(); break;
        case 2: slotQuery(); break;
        case 3: slotReset(); break;
        case 4: slotCancel(); break;
        case 5: slotShowUsers(); break;
        case 6: slotSelectImage(); break;
        case 7: slotPopulateCopiesEditor(); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
static const uint qt_meta_data_qtbook_journal[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_qtbook_journal[] = {
    "qtbook_journal\0\0slotCancel()\0"
};

const QMetaObject qtbook_journal::staticMetaObject = {
    { &qtbook_magazine::staticMetaObject, qt_meta_stringdata_qtbook_journal,
      qt_meta_data_qtbook_journal, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &qtbook_journal::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *qtbook_journal::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *qtbook_journal::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qtbook_journal))
        return static_cast<void*>(const_cast< qtbook_journal*>(this));
    return qtbook_magazine::qt_metacast(_clname);
}

int qtbook_journal::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = qtbook_magazine::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: slotCancel(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
