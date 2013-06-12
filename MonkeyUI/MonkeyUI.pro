#-------------------------------------------------
#
# Project created by QtCreator 2013-06-08T08:06:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MonkeyUI
TEMPLATE = app

INCLUDEPATH += ./ ../
SOURCES += main.cpp\
        MainWindow.cpp \
    ../MonkeyMessages/MessageFramer.cpp

HEADERS  += \
    MainWindow.h \
    ../MonkeyMessages/RemoteMessages.h \
    ../MonkeyMessages/ModeMessages.h \
    ../MonkeyMessages/MessageFramer.h

FORMS    += MainWindow.ui
