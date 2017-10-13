#-------------------------------------------------
#
# Project created by QtCreator 2017-10-13T14:57:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt-NeteaseMusic
TEMPLATE = app

include (./libsleek/libsleek.pri)

INCLUDEPATH += $$PWD/libsleek

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
