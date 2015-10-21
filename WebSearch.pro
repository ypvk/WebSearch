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
    dbutil.cpp \
    updateinfo.cpp \
    jobparamdialog.cpp \
    configdialog.cpp \
    engineinfo.cpp \
    clickinfo.cpp \
    engineconfig.cpp \
    aboutdialog.cpp \
    querythread.cpp

HEADERS  += mainwindow.h \
    browser.h \
    webpage.h \
    commonutils.h \
    networkcookiejar.h \
    dbutil.h \
    updateinfo.h \
    jobparamdialog.h \
    configdialog.h \
    engineinfo.h \
    clickinfo.h \
    engineconfig.h \
    aboutdialog.h \
    querythread.h

FORMS    += mainwindow.ui

RESOURCES += \
    jquery.qrc
