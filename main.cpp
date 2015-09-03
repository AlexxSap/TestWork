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
#include "benchmarks/BenchmarkWriteReadWithAnalogs.h"
#include "benchmarks/BenchmarkForComparingSqliteAndMysql.h"

#include "DataBaseInfo.h"

#else
#include <QApplication>
#include "mainwindow.h"
#endif

int main()
{   
#ifdef TEST
    int test = 0;
    DataBaseInfo::setDataBaseType(DataBaseInfo::SQLITE);

//    TestItemOperators testItemOperators;
//    test += QTest::qExec(&testItemOperators);

//    TestSaleHistoryDayOperators testSaleHistoryDayOperators;
//    test += QTest::qExec(&testSaleHistoryDayOperators);

//    TestSaleHistory testSaleHistory;
//    test += QTest::qExec(&testSaleHistory);

//    TestSaleHistoryGenerator testSaleHistoryGenerator;
//    test += QTest::qExec(&testSaleHistoryGenerator);

//    TestSaleHistoryParser testSaleHistoryParser;
//    test += QTest::qExec(&testSaleHistoryParser);

//    TestCsvFile testCsvFile;
//    test += QTest::qExec(&testCsvFile);

//    TAnalogs tAnalogs;
//    test += QTest::qExec(&tAnalogs);

//    TAnalogsTable tAnalogsTable;
//    test += QTest::qExec(&tAnalogsTable);

//    TAnalogsReader tAnalogsReader;
//    test += QTest::qExec(&tAnalogsReader);

//    TAnalogsFromFile tAnalogsFromFile;
//    test += QTest::qExec(&tAnalogsFromFile);

//    TAnalogsTableGenerator tAnalogsTableGenerator;
//    test += QTest::qExec(&tAnalogsTableGenerator);

//    TestSalesHistoryStreamReader testSalesHistoryStreamReader;
//    test += QTest::qExec(&testSalesHistoryStreamReader);

//    TSalesHistoryReaderWithAnalogs tSalesHistoryReaderWithAnalogs;
//    test += QTest::qExec(&tSalesHistoryReaderWithAnalogs);

    if(test == 0)
    {
//        BenchmarkWriteRead::run(720, 10, 10);

//        BenchmarkWriteRead::run(720, 10, 100);
//        BenchmarkWriteRead::run(720, 10, 1000);
//        BenchmarkWriteRead::run(720, 10, 10000);

//        BenchmarkWriteRead::runForBuffer(1000000);
//        BenchmarkAnalogsReader::run(5, 10);
//        BenchmarkWriteReadWithAnalogs::run(720, 10, 100, 5, 5);

/*
Сравнение версий
SQLITE                                      MYSQL
BenchmarkForComparingSqliteAndMysql::run(720, 10, 10);
1477           62                           6490           496
16MB (4MB)     15MB (1MB)                   42MB (26MB)   17MB (0MB)
BenchmarkWriteRead::run(720, 10, 100);
12886           647                         54311           801
255MB (242MB)   17MB (2MB)                  255MB (241MB)   25MB (0MB)
*/
        BenchmarkForComparingSqliteAndMysql::run(720, 10, 10);
        BenchmarkForComparingSqliteAndMysql::run(720, 10, 100);

        BenchmarkForComparingSqliteAndMysql::runWithAnalogs(720, 10, 10, 2, 2);
        BenchmarkForComparingSqliteAndMysql::runWithAnalogs(720, 10, 100, 5, 5);
    }
    return test;
#else
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
#endif
}
