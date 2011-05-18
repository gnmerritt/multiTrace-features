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
	../../src/*.cpp \
	layerviewer.cpp \
	cortexthread.cpp

HEADERS  += mainwindow.h \
	cortexviewer.hpp \
	../../src/*.hpp \
	layerviewer.hpp \
	QtUtility.hpp \
	cortexthread.hpp

FORMS    += mainwindow.ui \
	cortexviewer.ui \
	layerviewer.ui

OTHER_FILES +=

LIBS += /Users/nathan/Library/boost_1_46_0/stage/lib/libboost_serialization.a

INCLUDEPATH += /Users/nathan/Library/boost \
	../../src
