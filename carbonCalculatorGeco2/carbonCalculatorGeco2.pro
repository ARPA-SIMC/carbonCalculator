QT += core
QT -= gui

CONFIG += c++11

TARGET = carbonCalculatorGeco2
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

CONFIG(release, debug|release) {
    LIBS += -L../cabonCalculator/release -lcarboncalculator
} else {
    LIBS += -L../cabonCalculator/debug -lcarboncalculator
}

INCLUDEPATH += ../carbonCalculator


SOURCES += main.cpp
