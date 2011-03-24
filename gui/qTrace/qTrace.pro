#-------------------------------------------------
#
# Project created by QtCreator 2011-02-15T15:55:44
#
#-------------------------------------------------

QT       += core gui

TARGET = qTrace
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    cortexviewer.cpp \
    ../../src/*.cpp

HEADERS  += mainwindow.h \
    cortexviewer.h \
    ../../src/*.hpp

FORMS    += mainwindow.ui \
    cortexviewer.ui

OTHER_FILES +=

LIBS += /Users/nathan/Library/boost_1_46_0/stage/lib/libboost_serialization.a

INCLUDEPATH += /Users/nathan/Library/boost \
    ../../src
