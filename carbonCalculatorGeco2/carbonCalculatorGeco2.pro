QT += core sql
QT -= gui

TARGET = carbonCalculator
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
    dbOutput.cpp \
    inputOutput.cpp \
    carbonCredits.cpp

HEADERS += \
    dbOutput.h \
    inputOutput.h
