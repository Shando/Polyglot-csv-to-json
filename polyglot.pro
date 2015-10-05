#-------------------------------------------------
#
# Project created by QtCreator 2015-09-24T11:11:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = polyglot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    csv_parser.cpp

HEADERS  += mainwindow.h \
    ACSVParser.h \
    csv_parser.hpp

FORMS    += mainwindow.ui

RESOURCES += \
    polyglot.qrc
