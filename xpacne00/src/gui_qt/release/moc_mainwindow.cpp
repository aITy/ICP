/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Thu 9. May 04:29:12 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      21,   11,   11,   11, 0x08,
      39,   37,   11,   11, 0x08,
      78,   11,   11,   11, 0x08,
      92,   11,   11,   11, 0x08,
     127,   11,   11,   11, 0x08,
     159,   11,   11,   11, 0x08,
     189,   11,   11,   11, 0x08,
     220,   11,   11,   11, 0x08,
     249,   11,   11,   11, 0x08,
     284,  278,   11,   11, 0x08,
     323,   11,   11,   11, 0x08,
     340,   11,   11,   11, 0x08,
     356,   11,   11,   11, 0x08,
     375,   11,   11,   11, 0x08,
     390,   11,   11,   11, 0x08,
     411,   11,   11,   11, 0x08,
     423,   11,  418,   11, 0x08,
     433,   11,  418,   11, 0x08,
     443,  278,   11,   11, 0x08,
     485,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0listen()\0gotConnection()\0"
    ",\0gotInviteSlot(Player::color_t,QString)\0"
    "gotExitSlot()\0on_spinBox_Delay_valueChanged(int)\0"
    "on_pushButton_Forward_clicked()\0"
    "on_pushButton_Pause_clicked()\0"
    "on_pushButton_Rewind_clicked()\0"
    "on_pushButton_Play_clicked()\0"
    "on_pushButton_Stop_clicked()\0index\0"
    "on_tabWidget_Games_currentChanged(int)\0"
    "createLocalCpu()\0createLocalVs()\0"
    "showNewNetDialog()\0openFromFile()\0"
    "openReplayFromFile()\0help()\0bool\0"
    "saveIcp()\0saveXml()\0"
    "on_tabWidget_Games_tabCloseRequested(int)\0"
    "newNetworkGame(QStringList)\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    if (!strcmp(_clname, "Ui::MainWindow"))
        return static_cast< Ui::MainWindow*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: listen(); break;
        case 1: gotConnection(); break;
        case 2: gotInviteSlot((*reinterpret_cast< Player::color_t(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: gotExitSlot(); break;
        case 4: on_spinBox_Delay_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: on_pushButton_Forward_clicked(); break;
        case 6: on_pushButton_Pause_clicked(); break;
        case 7: on_pushButton_Rewind_clicked(); break;
        case 8: on_pushButton_Play_clicked(); break;
        case 9: on_pushButton_Stop_clicked(); break;
        case 10: on_tabWidget_Games_currentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: createLocalCpu(); break;
        case 12: createLocalVs(); break;
        case 13: showNewNetDialog(); break;
        case 14: openFromFile(); break;
        case 15: openReplayFromFile(); break;
        case 16: help(); break;
        case 17: { bool _r = saveIcp();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 18: { bool _r = saveXml();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 19: on_tabWidget_Games_tabCloseRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: newNetworkGame((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::listen()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
