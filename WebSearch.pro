#-------------------------------------------------
#
# Project created by QtCreator 2015-09-21T21:55:42
#
#-------------------------------------------------

QT       += core gui webkit network

TARGET = WebSearch
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    browser.cpp

HEADERS  += mainwindow.h \
    browser.h

FORMS    += mainwindow.ui

RESOURCES += \
    jquery.qrc
