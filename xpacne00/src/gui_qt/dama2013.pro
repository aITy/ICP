QT += gui network
CONFIG += qt xml console debug  # console opens cout, cerr
CONFIG -= app_bundle  # make console app work under MacOSX

DEFINES += QT_NO_KEYWORDS
QMAKE_CXXFLAGS += -std=c++11

HEADERS += app.h

SOURCES += main.cpp \
           app.cpp

# vim: set ft=make:
