TEMPLATE = app
QMAKE_CXXFLAGS +=  -std=gnu++1z -save-temps
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    paginator.cpp

HEADERS += \
    test_runner.h
