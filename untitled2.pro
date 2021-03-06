#-------------------------------------------------
#
# Project created by QtCreator 2012-03-04T23:24:55
# By Andrey Shmelev e-mail: andrshmel@gmail.com +79227112858
#-------------------------------------------------

QT += core gui script
QT += serialport
QT += testlib

VERSION = 0.5.5

Q_OS_LINUX = 1

QT_IM_MODULE=mockup
CONFIG += mockup

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
CONFIG += c++11

TARGET = untitled2
INCLUDEPATH += 3rdparty/libmodbus 3rdparty/qextserialport
TEMPLATE = app

SOURCES += main.cpp \
    qcustomplot.cpp \
    updategraph.cpp \
    keyboard.cpp \
    eventfilter.cpp \
    uartdriver.cpp \
    readoptionsfromfile.cpp \
    messages.cpp \
    writetofile.cpp \
    metrologicalcalc.cpp \
    mathresolver.cpp \
    options_gui.cpp \
    options_logic.cpp \
    mainwindow_gui.cpp \
    mainwindow_logic.cpp \
    painting.cpp \
    worker.cpp \
    3rdparty/libmodbus/src/modbus-data.c \
    3rdparty/libmodbus/src/modbus-rtu.c \
    3rdparty/libmodbus/src/modbus-tcp.c \
    3rdparty/libmodbus/src/modbus.c \
    3rdparty/qextserialport/qextserialport.cpp \
    stackedoptions.cpp \
    channelOptions.cpp

unix:SOURCES += 3rdparty/qextserialport/posix_qextserialport.cpp	\
                3rdparty/qextserialport/qextserialenumerator_unix.cpp

unix:DEFINES += _TTY_POSIX_

win32:SOURCES += 3rdparty/qextserialport/win_qextserialport.cpp \
                        3rdparty/qextserialport/qextserialenumerator_win.cpp
win32:DEFINES += _TTY_WIN_  WINVER=0x0501
win32:LIBS += -lsetupapi -lwsock32
win32:LIBS += -lws2_32

HEADERS  += mainwindow.h \
         qcustomplot.h \
    updategraph.h \
    options.h \
    keyboard.h \
    messages.h \
    uartdriver.h \
    metrologicalcalc.h \
    mathresolver.h \
    worker.h \
    defines.h \
    3rdparty/qextserialport/qextserialenumerator.h \
    3rdparty/qextserialport/qextserialport.h \
    device.h \
    stackedoptions.h \
    channelOptions.h

FORMS    += mainwindow.ui \
    options.ui \
    keyboard.ui \
    messages.ui \
    stackedoptions.ui
target.path  = /opt
INSTALLS    += target
DISTFILES += \
    ../build-untitled2-MYD-Release/Makefile \
    translator_en.ts \
    untitled_2_de.ts \
    untitled_2_en.ts \
    untitled_2_ru.ts \
    untitled2_de.ts \
    untitled2_en.ts \
    untitled2_ru.ts


TRANSLATIONS +=  untitled2_en.ts \
                 untitled2_ru.ts \
                 untitled2_de.ts
