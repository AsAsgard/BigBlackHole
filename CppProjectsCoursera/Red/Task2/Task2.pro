TEMPLATE = app
QMAKE_CXXFLAGS += -save-temps
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    update_field.cpp

HEADERS += \
    test_runner.h \
    airline_ticket.h
