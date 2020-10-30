#-------------------------------------------------
#
# Project created by QtCreator 2020-10-06T07:09:17
#
#-------------------------------------------------

QT      += sql
QT      -= gui

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

SOURCES += \
    carbonCalculator.cpp \
    csvUtilities.cpp \
    dbQueries.cpp \
    dbUtilities.cpp \
    energyManagement.cpp \
    fertilisationAndSoil.cpp \
    soilmanagement.cpp

HEADERS += \
    basicStructures.h \
    carbonCalculator.h \
    csvUtilities.h \
    dbQueries.h \
    dbUtilities.h \
    energyManagement.h \
    fertilisationAndSoil.h \
    soilManagement.h
