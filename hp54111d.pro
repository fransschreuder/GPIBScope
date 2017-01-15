#-------------------------------------------------
#
# Project created by QtCreator 2017-01-11T15:53:59
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GPIBScope
TEMPLATE = app


SOURCES += main.cpp\
        hpscope.cpp \
    gpib.cpp \
    hp54111d.cpp

HEADERS  += hpscope.h \
    gpib.h \
    hp54111d.h

FORMS    += hpscope.ui
