#-------------------------------------------------
#
# Project created by QtCreator 2013-06-08T08:06:30
#
#-------------------------------------------------

QT       += core gui
GRAPE_DIR = ../../grape
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MonkeyUI
TEMPLATE = app

DEFINES +=
win32: DEFINES += GRAPECORE_DLL GRAPEIO_DLL GRAPETIMING_DLL GRAPEUTILS_DLL UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS

INCLUDEPATH += \
    ./ \
    ../ \
    $${GRAPE_DIR}

LIBS += -L$${GRAPE_DIR}/lib

CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
    win32:LIBS += -lws2_32 -lUser32 -lGrapeTimingd0 -lGrapeCored0 -lGrapeIod0 -lGrapeUtilsd0 -llcm
    else:unix: LIBS += -lGrapeIod -lGrapeTimingd -lGrapeCored -lGrapeUtilsd -llcm -lpthread -lrt
} else {
    win32:LIBS += -lws2_32 -lUser32 -lGrapeTiming0 -lGrapeCore0 -lGrapeIo0 -lGrapeUtils0 -llcm
    else:unix: LIBS += -lGrapeIo -lGrapeTiming -lGrapeCore -lGrapeUtils -llcm -lpthread -lrt
}


SOURCES += main.cpp\
        MainWindow.cpp \
    ../MonkeyMessages/MessageFramer.cpp \
    ConsoleClient.cpp

HEADERS  += \
    MainWindow.h \
    ../MonkeyMessages/RemoteMessages.h \
    ../MonkeyMessages/ModeMessages.h \
    ../MonkeyMessages/MessageFramer.h \
    ConsoleClient.h

FORMS    += MainWindow.ui
