QT += core
QT -= gui

CONFIG += c++11

TARGET = carbonCalculatorGeco2
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

CONFIG(release, debug|release) {
    LIBS += -L../cabonCalculator/release -lcarbonCalculator
} else {
    LIBS += -L../cabonCalculator/debug -lcarbonCalculator
}

INCLUDEPATH += ../carbonCalculator


SOURCES += main.cpp
