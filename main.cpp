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
#include "benchmarks/BenchmarkWriteRead.h"
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

    TestSalesHistoryStreamReader testSalesHistoryStreamReader;
    test += QTest::qExec(&testSalesHistoryStreamReader);

    TestSaleHistoryGenerator testSaleHistoryGenerator;
    test += QTest::qExec(&testSaleHistoryGenerator);

    TestSaleHistoryParser testSaleHistoryParser;
    test += QTest::qExec(&testSaleHistoryParser);

    TestCsvFile testCsvFile;
    test += QTest::qExec(&testCsvFile);

    if(test == 0)
    {
        BenchmarkWriteRead::run(720, 10, 10);
        BenchmarkWriteRead::run(720, 10, 100);
//        BenchmarkWriteRead::run(720, 10, 1000);
//        BenchmarkWriteRead::run(40, 10, 1000);

//        BenchmarkWriteRead::run(30, 1, 100);
//        BenchmarkWriteRead::run(90, 1, 100);
//        BenchmarkWriteRead::run(30, 10, 100);
//        BenchmarkWriteRead::run(30, 1, 1000);
    }
    return test;
#else
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
#endif
}
