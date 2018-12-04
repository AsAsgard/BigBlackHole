TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    condition_parser.cpp \
    condition_parser_test.cpp \
    token.cpp \
    database.cpp \
    node.cpp \
    date.cpp \
    main.cpp \
    test_runner.cpp

HEADERS += \
    condition_parser.h \
    token.h \
    database.h \
    node.h \
    date.h \
    test_runner.h
