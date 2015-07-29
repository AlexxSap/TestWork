#-------------------------------------------------
#
# Project created by QtCreator 2015-07-29T09:27:38
#
#-------------------------------------------------

QT       += core gui testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestWork
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    TestFileReder.cpp \
    CsvFileReader.cpp

HEADERS  += mainwindow.h \
    TestFileReder.h \
    FileReader.h \
    CsvFileReader.h
