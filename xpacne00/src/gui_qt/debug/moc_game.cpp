/****************************************************************************
** Meta object code from reading C++ file 'game.h'
**
** Created: Thu 9. May 01:37:04 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../share/game.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'game.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QTimerImproved[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      29,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QTimerImproved[] = {
    "QTimerImproved\0\0newTimeout()\0"
    "resendTimeout()\0"
};

const QMetaObject QTimerImproved::staticMetaObject = {
    { &QTimer::staticMetaObject, qt_meta_stringdata_QTimerImproved,
      qt_meta_data_QTimerImproved, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QTimerImproved::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QTimerImproved::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QTimerImproved::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QTimerImproved))
        return static_cast<void*>(const_cast< QTimerImproved*>(this));
    return QTimer::qt_metacast(_clname);
}

int QTimerImproved::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTimer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: newTimeout(); break;
        case 1: resendTimeout(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void QTimerImproved::newTimeout()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_Game[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
       6,    5,    5,    5, 0x05,
      18,   16,    5,    5, 0x05,
      53,    5,    5,    5, 0x05,

 // slots: signature, parameters, type, tag, flags
      63,    5,    5,    5, 0x0a,
      78,    5,    5,    5, 0x0a,
      91,    5,    5,    5, 0x0a,
     109,    5,    5,    5, 0x0a,
     122,    5,    5,    5, 0x0a,
     155,    5,    5,    5, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Game[] = {
    "Game\0\0refresh()\0,\0gotInvite(Player::color_t,QString)\0"
    "gotExit()\0gotConnected()\0gotNewData()\0"
    "gotDisconnected()\0gotTimeout()\0"
    "dispatchUserResponseInvite(bool)\0"
    "dispatchUserResponseExit()\0"
};

const QMetaObject Game::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Game,
      qt_meta_data_Game, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Game::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Game::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Game::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Game))
        return static_cast<void*>(const_cast< Game*>(this));
    return QObject::qt_metacast(_clname);
}

int Game::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: refresh(); break;
        case 1: gotInvite((*reinterpret_cast< Player::color_t(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: gotExit(); break;
        case 3: gotConnected(); break;
        case 4: gotNewData(); break;
        case 5: gotDisconnected(); break;
        case 6: gotTimeout(); break;
        case 7: dispatchUserResponseInvite((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: dispatchUserResponseExit(); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void Game::refresh()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Game::gotInvite(Player::color_t _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Game::gotExit()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
