#-------------------------------------------------
#
# Project created by QtCreator 2012-09-26T17:29:44
#
#-------------------------------------------------

QT       += core gui

TARGET = LcmsTest
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

macx: LIBS += -L$$PWD/../../../../../opt/local/lib/ -llcms2

INCLUDEPATH += $$PWD/../../../../../opt/local/include
DEPENDPATH += $$PWD/../../../../../opt/local/include
