#-------------------------------------------------
#
# Project created by QtCreator 2015-07-29T09:27:38
#
#-------------------------------------------------

QT       += core gui testlib sql widgets

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestWork
TEMPLATE = app

LIBS += -lpsapi


SOURCES += main.cpp\
        mainwindow.cpp \
    DataBase.cpp \
    Item.cpp \
    SaleHistoryGenerator.cpp \
    tests/TestItemOperators.cpp \
    tests/TestSaleHistoryDayOperators.cpp \
    SaleHistoryDay.cpp \
    tests/TestSaleHistory.cpp \
    SaleHistory.cpp \
    tests/TestSaleHistoryGenerator.cpp \
    SalesHistoryStreamReader.cpp \
    tests/TestSalesHistoryStreamReader.cpp \
    SaleHistoryWriter.cpp \
    SaleHistoryParser.cpp \
    tests/TestSaleHistoryParser.cpp \
    tests/TestUtility.cpp \
    CsvFile.cpp \
    tests/TestCsvFile.cpp \
    benchmarks/BenchmarkWriteRead.cpp \
    RamTracker/RamTracker.cpp \
    RamTracker/Utils.cpp \
    tests/TAnalogs.cpp \
    Analogs.cpp \
    tests/TAnalogsTable.cpp \
    AnalogsTable.cpp

HEADERS  += mainwindow.h \
    DataBase.h \
    Item.h \
    SaleHistoryGenerator.h \
    tests/TestItemOperators.h \
    tests/TestSaleHistoryDayOperators.h \
    SaleHistoryDay.h \
    tests/TestSaleHistory.h \
    SaleHistory.h \
    tests/TestSaleHistoryGenerator.h \
    SalesHistoryStreamReader.h \
    tests/TestSalesHistoryStreamReader.h \
    SaleHistoryWriter.h \
    SaleHistoryParser.h \
    tests/TestSaleHistoryParser.h \
    tests/TestUtility.h \
    CsvFile.h \
    tests/TestCsvFile.h \
    benchmarks/BenchmarkWriteRead.h \
    RamTracker/RamTracker.h \
    RamTracker/Utils.h \
    tests/TAnalogs.h \
    Analogs.h \
    tests/TAnalogsTable.h \
    AnalogsTable.h
