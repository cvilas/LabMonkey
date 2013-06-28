TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

GRAPE_DIR = ../../../../../grape

INCLUDEPATH += $${PWD} $${GRAPE_DIR}
LIBS += -L$${GRAPE_DIR}/lib
LIBS += -lGrapeIo -lGrapeTiming -lGrapeUtils -lGrapeCore -lpthread -lrt

SOURCES += main.cpp \
    transport.cpp \
    motor.cpp \
    labmonkey.cpp

DESTDIR = $${PWD}/../bin
DLLDESTDIR = $${PWD}/../bin

build_pass:CONFIG(debug, release|debug) {
    TARGET = $$join(TARGET,,,d)
} else {
    TARGET = $$join(TARGET,,,)
}

HEADERS += \
    transport.h \
    motor.h \
    labmonkey.h




