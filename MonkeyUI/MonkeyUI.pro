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
    win32:LIBS += -lws2_32 -lUser32 -lGrapeTimingd0 -lGrapeCored0 -lGrapeIod0 -lGrapeUtilsd0
    else:unix: LIBS += -lGrapeIod -lGrapeTimingd -lGrapeCored -lGrapeUtilsd -lpthread -lrt
} else {
    win32:LIBS += -lws2_32 -lUser32 -lGrapeTiming0 -lGrapeCore0 -lGrapeIo0 -lGrapeUtils0
    else:unix: LIBS += -lGrapeIo -lGrapeTiming -lGrapeCore -lGrapeUtils -lpthread -lrt
}


SOURCES += main.cpp\
        MainWindow.cpp \
    ../MonkeyMessages/MessageFramer.cpp \
    ConsoleClient.cpp \
    IpPortDlg.cpp \
    ModeSelectUi.cpp

HEADERS  += \
    MainWindow.h \
    ../MonkeyMessages/RemoteMessages.h \
    ../MonkeyMessages/ModeMessages.h \
    ../MonkeyMessages/MessageFramer.h \
    ConsoleClient.h \
    IpPortDlg.h \
    ModeSelectUi.h \
    ../MonkeyMessages/WaypointMessages.h \
    ../MonkeyMessages/SpeedMessages.h \
    ../MonkeyMessages/PositionMessages.h

FORMS    += MainWindow.ui \
    IpPortDlg.ui \
    ModeSelectUi.ui
