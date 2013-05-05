QT += network xml
QT -= gui
CONFIG += qt console debug  # console opens cout, cerr
CONFIG -= app_bundle  # make console app work under MacOSX

DEFINES += QT_NO_KEYWORDS
QMAKE_CXXFLAGS += -std=c++11

HEADERS += app.h \
           ../share/game.h

SOURCES += main.cpp \
           app.cpp \
           ../share/game.cpp

# vim: set ft=make:
