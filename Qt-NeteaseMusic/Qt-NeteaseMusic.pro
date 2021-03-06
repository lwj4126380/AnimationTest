#-------------------------------------------------
#
# Project created by QtCreator 2017-10-13T14:57:13
#
#-------------------------------------------------

QT       += core gui svg xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt-NeteaseMusic
TEMPLATE = app

CONFIG += c++11

include (./libsleek/libsleek.pri)
include (./audio/decoder/decoders.pri)
include (./audio/dataprovider/dataprovider.pri)

INCLUDEPATH += $$PWD/libsleek

INCLUDEPATH += $$PWD/widgets/genericcontrols
INCLUDEPATH += $$PWD/utils
SOURCES += main.cpp\
        mainwindow.cpp \
    widgets/genericcontrols/hoverablewidget.cpp \
    widgets/genericcontrols/searchlineedit.cpp \
    widgets/genericcontrols/headclickablelistwidget.cpp \
    widgets/uicomponent/sidebar.cpp \
    utils/qsshelper.cpp \
    widgets/uicomponent/playwidget.cpp

HEADERS  += mainwindow.h \
    widgets/genericcontrols/hoverablewidget.h \
    widgets/genericcontrols/searchlineedit.h \
    widgets/genericcontrols/headclickablelistwidget.h \
    widgets/uicomponent/sidebar.h \
    utils/qsshelper.h \
    widgets/uicomponent/playwidget.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
TRANSLATIONS += zhCN.ts
RC_FILE=icon.rc
