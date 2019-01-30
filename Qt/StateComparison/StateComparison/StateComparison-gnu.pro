#-------------------------------------------------
#
# Project created by QtCreator 2019-01-30T12:49:34
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StateComparison-gnu
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    cdatastate.cpp \
    cf_colorchanger.cpp \
    comparisonfield.cpp \
    cstate.cpp \
    demoareacf.cpp \
    demoareakv.cpp \
    fa_box.cpp \
    filebrowser.cpp \
    kv_distribution.cpp \
    kv_settings.cpp \
    main.cpp \
    pathandfiles.cpp \
    qcustomplot.cpp \
    renderarea.cpp

HEADERS += \
    cdatastate.h \
    cf_colorchanger.h \
    comparisonfield.h \
    cstate.h \
    demoareacf.h \
    demoareakv.h \
    extfunctions.h \
    fa_box.h \
    filebrowser.h \
    kv_distribution.h \
    kv_settings.h \
    pathandfiles.h \
    qcustomplot.h \
    renderarea.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    cf_colorchanger.ui \
    comparisonfield.ui \
    filebrowser.ui \
    kv_distribution.ui \
    kv_settings.ui

RESOURCES += \
    filebrowser.qrc

RC_FILE = myicon.rc
