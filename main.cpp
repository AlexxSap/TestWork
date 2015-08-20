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

    TestSaleHistoryGenerator testSaleHistoryGenerator;
    test += QTest::qExec(&testSaleHistoryGenerator);

    TestSaleHistoryParser testSaleHistoryParser;
    test += QTest::qExec(&testSaleHistoryParser);

    TestCsvFile testCsvFile;
    test += QTest::qExec(&testCsvFile);

    TestSalesHistoryStreamReader testSalesHistoryStreamReader;
    test += QTest::qExec(&testSalesHistoryStreamReader);

    if(test == 0)
    {
//        BenchmarkWriteRead::run(720, 10, 10);   //  40 mb
//        BenchmarkWriteRead::run(720, 10, 100);  // 80 mb
//        BenchmarkWriteRead::run(720, 10, 1000);   //58 mb 300 mb
//        BenchmarkWriteRead::run(720, 10, 100);//14
//        BenchmarkWriteRead::run(720, 10, 200);//20
//        BenchmarkWriteRead::run(720, 10, 300);//40


//        BenchmarkWriteRead::run(30, 1, 100);
//        BenchmarkWriteRead::run(90, 1, 100);
//        BenchmarkWriteRead::run(30, 10, 100);
//        BenchmarkWriteRead::run(30, 1, 1000);

//        BenchmarkWriteRead::runForBuffer(1000000);
//        BenchmarkWriteRead::runForBuffer(100000);
//        BenchmarkWriteRead::runForBuffer(10000);
    }
    return test;
#else
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
#endif
}
