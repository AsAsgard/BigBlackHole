TEMPLATE = app
QMAKE_CXXFLAGS += -save-temps
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    print_values.cpp

HEADERS += \
    test_runner.h
