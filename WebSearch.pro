#-------------------------------------------------
#
# Project created by QtCreator 2015-09-21T21:55:42
#
#-------------------------------------------------

QT       += core gui webkit network sql

TARGET = WebSearch
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    browser.cpp \
    webpage.cpp \
    commonutils.cpp \
    networkcookiejar.cpp \
    dbutil.cpp

HEADERS  += mainwindow.h \
    browser.h \
    webpage.h \
    commonutils.h \
    networkcookiejar.h \
    dbutil.h

FORMS    += mainwindow.ui

RESOURCES += \
    jquery.qrc
