#-------------------------------------------------
#
# Project created by QtCreator 2020-10-06T07:09:17
#
#-------------------------------------------------

QT       -= gui

unix:{
    CONFIG(debug, debug|release) {
        TARGET = debug/carbonCalculator
    } else {
        TARGET = release/carbonCalculator
    }
}

win32:{
    TARGET = carbonCalculator
}

#TARGET = carbonCalculator

TEMPLATE = lib
CONFIG += staticlib

SOURCES += carboncalculator.cpp

HEADERS += carboncalculator.h
