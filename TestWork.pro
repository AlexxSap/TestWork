#-------------------------------------------------
#
# Project created by QtCreator 2015-07-29T09:27:38
#
#-------------------------------------------------

QT       += core gui testlib sql widgets

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestWork
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    CsvFileReader.cpp \
    tests/TestWriteReadFileToDB.cpp \
    CsvFileWriter.cpp \
    FileReader.cpp \
    FileWriter.cpp \
    SqliteAllDataDataSelector.cpp \
    SqliteSelByProductDataSelector.cpp \
    SqliteSelByDateDataSelector.cpp \
    DataBase.cpp \
    Item.cpp \
    SaleHistoryGenerator.cpp \
    tests/TestItemOperators.cpp

HEADERS  += mainwindow.h \
    FileReader.h \
    CsvFileReader.h \
    tests/TestWriteReadFileToDB.h \
    FileWriter.h \
    CsvFileWriter.h \
    SqliteAllDataDataSelector.h \
    SqliteSelByProductDataSelector.h \
    SqliteSelByDateDataSelector.h \
    DataBase.h \
    Item.h \
    SaleHistoryGenerator.h \
    tests/TestItemOperators.h
