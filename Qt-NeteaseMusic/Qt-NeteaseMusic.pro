#-------------------------------------------------
#
# Project created by QtCreator 2017-10-13T14:57:13
#
#-------------------------------------------------

QT       += core gui svg xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt-NeteaseMusic
TEMPLATE = app

CONFIG += c++11

include (./libsleek/libsleek.pri)

INCLUDEPATH += $$PWD/libsleek

INCLUDEPATH += $$PWD/widgets/genericcontrols

SOURCES += main.cpp\
        mainwindow.cpp \
    widgets/genericcontrols/svgbutton.cpp

HEADERS  += mainwindow.h \
    widgets/genericcontrols/svgbutton.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
