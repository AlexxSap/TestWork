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
    tests/TestFileReder.cpp \
    CsvFileReader.cpp \
    tests/TestWriteReadFileToDB.cpp \
    SqliteDataBase.cpp \
    CsvFileWriter.cpp \
    DataBase.cpp \
    FileReader.cpp \
    FileWriter.cpp \
    DataSelector.cpp \
    SqliteAllDataDataSelector.cpp \
    SqliteSelByProductDataSelector.cpp \
    SqliteSelByDateDataSelector.cpp

HEADERS  += mainwindow.h \
    tests/TestFileReder.h \
    FileReader.h \
    CsvFileReader.h \
    DataBase.h \
    tests/TestWriteReadFileToDB.h \
    SqliteDataBase.h \
    FileWriter.h \
    CsvFileWriter.h \
    DataSelector.h \
    SqliteAllDataDataSelector.h \
    SqliteSelByProductDataSelector.h \
    SqliteSelByDateDataSelector.h
