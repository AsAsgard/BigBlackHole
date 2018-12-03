TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    query.cpp \
    bus_manager.cpp \
    responses.cpp

HEADERS += \
    query.h \
    bus_manager.h \
    responses.h
