QT += core sql
QT -= gui

TARGET = carbonCalculatorTest
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../carbonCalculator


CONFIG(release, debug|release) {
    LIBS += -L../carbonCalculator/release -lcarbonCalculator
} else {
    LIBS += -L../carbonCalculator/debug -lcarbonCalculator
}


SOURCES += main.cpp \
    inputoutput.cpp

HEADERS += \
    inputoutput.h
