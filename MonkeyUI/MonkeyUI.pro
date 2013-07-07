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
    else: LIBS += -lGrapeIod -lGrapeTimingd -lGrapeCored -lGrapeUtilsd
    unix:!android: LIBS += -lpthread -lrt
} else {
    win32:LIBS += -lws2_32 -lUser32 -lGrapeTiming0 -lGrapeCore0 -lGrapeIo0 -lGrapeUtils0
    else: LIBS += -lGrapeIo -lGrapeTiming -lGrapeCore -lGrapeUtils
    unix:!android: LIBS += -lpthread -lrt
}


SOURCES += main.cpp\
        MainWindow.cpp \
    ../MonkeyMessages/MessageFramer.cpp \
    ConsoleClient.cpp \
    IpPortDlg.cpp \
    ModeSelectUi.cpp \
    PlayModeUi.cpp \
    TeachModeUi.cpp

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
    ../MonkeyMessages/PositionMessages.h \
    PlayModeUi.h \
    TeachModeUi.h

FORMS    += MainWindow.ui \
    IpPortDlg.ui \
    ModeSelectUi.ui \
    PlayModeUi.ui \
    TeachModeUi.ui

OTHER_FILES += \
    MonkeyUI.qss \
    android/res/values-ja/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values/strings.xml \
    android/res/values/libs.xml \
    android/res/layout/splash.xml \
    android/res/values-el/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/AndroidManifest.xml \
    android/libjava.so \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java \
    android/version.xml \
    android/Makefile \
    android/java.pro \
    android/res/values-ja/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values/strings.xml \
    android/res/values/libs.xml \
    android/res/layout/splash.xml \
    android/res/values-el/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/AndroidManifest.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java \
    android/version.xml

RESOURCES += \
    MonkeyUI.qrc
