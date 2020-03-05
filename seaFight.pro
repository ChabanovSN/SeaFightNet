#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T18:12:51
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = seaFight
TEMPLATE = app


SOURCES += main.cpp\
    field.cpp \
    myfield.cpp \
    enemyfield.cpp \
    serverwindow.cpp \
    clientdialog.cpp \
    compasplayer.cpp \
    window_sea_fight.cpp

HEADERS  += \
    field.h \
    myfield.h \
    enemyfield.h \
    serverwindow.h \
    clientdialog.h \
    compasplayer.h \
    window_sea_fight.h

FORMS    += \
    serverwindow.ui \
    clientdialog.ui

DISTFILES +=

RESOURCES += \
    res.qrc
