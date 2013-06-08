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
        MainWindow.cpp

HEADERS  += \
    MainWindow.h \
    ../RemoteMessages/RemoteMessages.h \
    ../RemoteMessages/ModeMessages.h

FORMS    += MainWindow.ui
