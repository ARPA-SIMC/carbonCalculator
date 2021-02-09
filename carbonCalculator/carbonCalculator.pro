#------------------------------------------------------
#
# Carbon Calculator
# developed in the framework of IT-HR GECO2 project
#
#-------------------------------------------------------

QT += core sql
QT -= gui

TARGET = carbonCalculator
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../carbonCalculatorLib


CONFIG(release, debug|release) {
    LIBS += -L../carbonCalculatorLib/release -lcarbonCalculatorLib
} else {
    LIBS += -L../carbonCalculatorLib/debug -lcarbonCalculatorLib
}


SOURCES += main.cpp \
    dbOutput.cpp \
    inputOutput.cpp \
    carbonCredits.cpp

HEADERS += \
    dbOutput.h \
    inputOutput.h
