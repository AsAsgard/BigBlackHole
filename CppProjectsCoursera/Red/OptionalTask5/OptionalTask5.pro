TEMPLATE = app
QMAKE_LFLAGS = -static -static-libgcc
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    express.cpp

HEADERS += \
    profile.h
