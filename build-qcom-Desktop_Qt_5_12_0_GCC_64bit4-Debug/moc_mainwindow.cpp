/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qcom/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[18];
    char stringdata0[389];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 30), // "on_actionWriteToFile_triggered"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 28), // "on_actionCleanPort_triggered"
QT_MOC_LITERAL(4, 72, 27), // "on_actionLoadfile_triggered"
QT_MOC_LITERAL(5, 100, 28), // "on_delayspinBox_valueChanged"
QT_MOC_LITERAL(6, 129, 22), // "on_actionAdd_triggered"
QT_MOC_LITERAL(7, 152, 23), // "on_actionExit_triggered"
QT_MOC_LITERAL(8, 176, 23), // "on_actionSave_triggered"
QT_MOC_LITERAL(9, 200, 22), // "on_obocheckBox_clicked"
QT_MOC_LITERAL(10, 223, 29), // "on_actionClearBytes_triggered"
QT_MOC_LITERAL(11, 253, 24), // "on_actionAbout_triggered"
QT_MOC_LITERAL(12, 278, 24), // "on_actionClose_triggered"
QT_MOC_LITERAL(13, 303, 23), // "on_actionOpen_triggered"
QT_MOC_LITERAL(14, 327, 21), // "on_clearUpBtn_clicked"
QT_MOC_LITERAL(15, 349, 21), // "on_sendmsgBtn_clicked"
QT_MOC_LITERAL(16, 371, 9), // "readMyCom"
QT_MOC_LITERAL(17, 381, 7) // "sendMsg"

    },
    "MainWindow\0on_actionWriteToFile_triggered\0"
    "\0on_actionCleanPort_triggered\0"
    "on_actionLoadfile_triggered\0"
    "on_delayspinBox_valueChanged\0"
    "on_actionAdd_triggered\0on_actionExit_triggered\0"
    "on_actionSave_triggered\0on_obocheckBox_clicked\0"
    "on_actionClearBytes_triggered\0"
    "on_actionAbout_triggered\0"
    "on_actionClose_triggered\0"
    "on_actionOpen_triggered\0on_clearUpBtn_clicked\0"
    "on_sendmsgBtn_clicked\0readMyCom\0sendMsg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x08 /* Private */,
       3,    0,   95,    2, 0x08 /* Private */,
       4,    0,   96,    2, 0x08 /* Private */,
       5,    1,   97,    2, 0x08 /* Private */,
       6,    0,  100,    2, 0x08 /* Private */,
       7,    0,  101,    2, 0x08 /* Private */,
       8,    0,  102,    2, 0x08 /* Private */,
       9,    0,  103,    2, 0x08 /* Private */,
      10,    0,  104,    2, 0x08 /* Private */,
      11,    0,  105,    2, 0x08 /* Private */,
      12,    0,  106,    2, 0x08 /* Private */,
      13,    0,  107,    2, 0x08 /* Private */,
      14,    0,  108,    2, 0x08 /* Private */,
      15,    0,  109,    2, 0x08 /* Private */,
      16,    0,  110,    2, 0x08 /* Private */,
      17,    0,  111,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
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

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_actionWriteToFile_triggered(); break;
        case 1: _t->on_actionCleanPort_triggered(); break;
        case 2: _t->on_actionLoadfile_triggered(); break;
        case 3: _t->on_delayspinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_actionAdd_triggered(); break;
        case 5: _t->on_actionExit_triggered(); break;
        case 6: _t->on_actionSave_triggered(); break;
        case 7: _t->on_obocheckBox_clicked(); break;
        case 8: _t->on_actionClearBytes_triggered(); break;
        case 9: _t->on_actionAbout_triggered(); break;
        case 10: _t->on_actionClose_triggered(); break;
        case 11: _t->on_actionOpen_triggered(); break;
        case 12: _t->on_clearUpBtn_clicked(); break;
        case 13: _t->on_sendmsgBtn_clicked(); break;
        case 14: _t->readMyCom(); break;
        case 15: _t->sendMsg(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
