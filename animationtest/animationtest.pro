#-------------------------------------------------
#
# Project created by QtCreator 2017-10-12T11:36:43
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = animationtest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    autoloadpicturelabel.cpp \
    bannerwidget.cpp

HEADERS  += mainwindow.h \
    autoloadpicturelabel.h \
    bannerwidget.h

FORMS    += mainwindow.ui
