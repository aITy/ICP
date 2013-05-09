QT += gui \
    network \
    xml
DEFINES += QT_FATAL_ASSERT
TEMPLATE = app
HEADERS += mainwindow.h \
    lightrock.h \
    gameboard.h \
    darkrock.h \
    canvas.h \
    ../share/game.h \
    connectdialog.h \
    acceptdialog.h \
    help.h
SOURCES += main.cpp \
    mainwindow.cpp \
    lightrock.cpp \
    gameboard.cpp \
    darkrock.cpp \
    canvas.cpp \
    ../share/game.cpp \
    connectdialog.cpp \
    acceptdialog.cpp \
    help.cpp
FORMS += mainwindow.ui \
    board.ui \
    connectdialog.ui \
    acceptdialog.ui \
    help.ui
RESOURCES += recources.qrc
OTHER_FILES += stop.png \
    rewind.png \
    play.png \
    pause.png \
    light_piece.png \
    forward.png \
    dark_piece.png \
    light_king.png \
    dark_king.png
