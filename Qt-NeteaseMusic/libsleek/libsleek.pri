win32 {
LIBS += gdi32.lib dwmapi.lib

SOURCES += $$PWD/qwinwidget.cpp \
    $$PWD/sleekwindowclass.cpp \
    $$PWD/sleekborderless.cpp

HEADERS += $$PWD/qwinwidget.h \
    $$PWD/sleekwindowclass.h \
    $$PWD/sleekborderless.h
}

SOURCES += \
    $$PWD/sleekwindow.cpp

HEADERS += \
    $$PWD/libsleek_global.h \
    $$PWD/sleekwindow.h
