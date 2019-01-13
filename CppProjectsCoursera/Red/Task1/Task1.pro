TEMPLATE = app
QMAKE_CXXFLAGS += -save-temps
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    sort_by.cpp

HEADERS += \
    airline_ticket.h \
    test_runner.h
