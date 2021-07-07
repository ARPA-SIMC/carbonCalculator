#------------------------------------------------------
#
# Carbon Calculator
# developed in the framework of IT-HR GECO2 project
#
#-------------------------------------------------------

QT += core sql
QT -= gui

TARGET = carbonCalculatorScript
CONFIG += console
CONFIG += _CRT_SECURE_NO_WARNINGS
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../carbonCalculatorLib


CONFIG(release, debug|release) {
    LIBS += -L../carbonCalculatorLib/release -lcarbonCalculatorLib
} else {
    LIBS += -L../carbonCalculatorLib/debug -lcarbonCalculatorLib
}


SOURCES += main.cpp

