#define TEST

#ifdef TEST
#include <QTest>
#include "tests/TestItemOperators.h"
#include "tests/TestSaleHistoryDayOperators.h"
#include "tests/TestSaleHistory.h"
#include "tests/TestSaleHistoryGenerator.h"
#include "tests/TestSaleHistoryParser.h"
#include "tests/TestSalesHistoryStreamReader.h"
#include "tests/TestCsvFile.h"
#include "tests/TAnalogs.h"
#include "tests/TAnalogsTable.h"
#include "tests/TAnalogsReader.h"
#include "tests/TAnalogsFromFile.h"
#include "tests/TAnalogsTableGenerator.h"
#include "tests/TSalesHistoryReaderWithAnalogs.h"

#include "benchmarks/BenchmarkWriteRead.h"
#include "benchmarks/BenchmarkAnalogsReader.h"

#include <QVector>

#else
#include <QApplication>
#include "mainwindow.h"
#endif

int main()
{   
#ifdef TEST
    int test = 0;
    TestItemOperators testItemOperators;
    test += QTest::qExec(&testItemOperators);

    TestSaleHistoryDayOperators testSaleHistoryDayOperators;
    test += QTest::qExec(&testSaleHistoryDayOperators);

    TestSaleHistory testSaleHistory;
    test += QTest::qExec(&testSaleHistory);

    TestSaleHistoryGenerator testSaleHistoryGenerator;
    test += QTest::qExec(&testSaleHistoryGenerator);

    TestSaleHistoryParser testSaleHistoryParser;
    test += QTest::qExec(&testSaleHistoryParser);

    TestCsvFile testCsvFile;
    test += QTest::qExec(&testCsvFile);

    TestSalesHistoryStreamReader testSalesHistoryStreamReader;
    test += QTest::qExec(&testSalesHistoryStreamReader);

    TAnalogs tAnalogs;
    test += QTest::qExec(&tAnalogs);

    TAnalogsTable tAnalogsTable;
    test += QTest::qExec(&tAnalogsTable);

    TAnalogsReader tAnalogsReader;
    test += QTest::qExec(&tAnalogsReader);

    TAnalogsFromFile tAnalogsFromFile;
    test += QTest::qExec(&tAnalogsFromFile);

    TAnalogsTableGenerator tAnalogsTableGenerator;
    test += QTest::qExec(&tAnalogsTableGenerator);

    TSalesHistoryReaderWithAnalogs tSalesHistoryReaderWithAnalogs;
    test += QTest::qExec(&tSalesHistoryReaderWithAnalogs);

    if(test == 0)
    {
//        BenchmarkWriteRead::run(720, 10, 10);
//        BenchmarkWriteRead::run(720, 10, 100);
//        BenchmarkWriteRead::run(720, 10, 1000);
//        BenchmarkWriteRead::run(720, 10, 10000);

//        BenchmarkWriteRead::run(30, 1, 100);
//        BenchmarkWriteRead::run(90, 1, 100);
//        BenchmarkWriteRead::run(30, 10, 100);
//        BenchmarkWriteRead::run(30, 1, 1000);

//        BenchmarkWriteRead::runForBuffer(1000000);
//        BenchmarkWriteRead::runForBuffer(100000);
//        BenchmarkWriteRead::runForBuffer(10000);

//        BenchmarkAnalogsReader::run(500, 100);
//        BenchmarkAnalogsReader::run(500, 1000);
//        BenchmarkAnalogsReader::run(1000, 1000);

    }
    return test;
#else
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
#endif
}
