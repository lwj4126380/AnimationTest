#-------------------------------------------------
#
# Project created by QtCreator 2017-10-31T11:04:33
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AudioDecoderTest
TEMPLATE = app

LIBS += -lavcodec -lavformat -lavutil -lswresample

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    BlockingQueue.h

FORMS    += mainwindow.ui
