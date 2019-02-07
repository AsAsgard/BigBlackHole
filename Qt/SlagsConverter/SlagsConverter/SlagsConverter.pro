#-------------------------------------------------
#
# Project created by QtCreator 2019-02-03T16:54:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SlagsConverter
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

LIBS += -lgfortran

VPATH += ../LibShlaki

#FORTRAN_SOURCES += \
#    ArrayOf10Numbers.f90    \
#    ShlakiDataModule.f90    \
#    Read_ShlakiNew.f90  \
#    Read_ShlakiOld.f90  \
#    Shlaki_DataRewrite.f90  \
#    Shlaki_ListConversion.f90   \
#    Shlaki_Normalization.f90    \
#    TWIC.f90

#gfortran.output = ${QMAKE_FILE_BASE}.o
#gfortran.commands = gfortran ${QMAKE_FILE_NAME} -c -o ${QMAKE_FILE_OUT}
#gfortran.input = FORTRAN_SOURCES
#QMAKE_EXTRA_COMPILERS += gfortran

SOURCES += \
        main.cpp \
    converterslags.cpp

HEADERS += \
    converterslags.h

FORMS += \
    converterslags.ui

CONFIG(debug, debug|release):LIBS += -L../../LibShlaki/Debug/  -lLibShlakid

CONFIG(release, debug|release):LIBS += -L../../LibShlaki/Release/  -lLibShlaki


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_FILE += myicon.rc
