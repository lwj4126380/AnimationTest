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
LIBS += $$PWD/portaudio/portaudio_x86.lib

INCLUDEPATH += $$PWD/portaudio

SOURCES += main.cpp\
        mainwindow.cpp \
    Packet.cpp \
    PacketBuffer.cpp \
    audiodemuxer.cpp

HEADERS  += mainwindow.h \
    BlockingQueue.h \
    Packet.h \
    PacketBuffer.h \
    ring.h \
    audiodemuxer.h

FORMS    += mainwindow.ui
