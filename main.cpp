#include "mainwindow.h"
#include <QApplication>
#include <QTest>

#include "tests/TestItemOperators.h"
#include "tests/TestSaleHistoryDayOperators.h"
#include "tests/TestSaleHistory.h"
#include "tests/TestSaleHistoryGenerator.h"
#include "tests/TestSaleHistoryParser.h"
#include "tests/TestSalesHistoryStreamReader.h"
#include "tests/TestCsvFile.h"
#include "tests/BenchmarkReadWriteToDb.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(argc == 2 && QString(argv[1]) == "-t")
    {
        TestItemOperators testItemOperators;
        QTest::qExec(&testItemOperators);

        TestSaleHistoryDayOperators testSaleHistoryDayOperators;
        QTest::qExec(&testSaleHistoryDayOperators);

        TestSaleHistory testSaleHistory;
        QTest::qExec(&testSaleHistory);

        TestSaleHistoryGenerator testSaleHistoryGenerator;
        QTest::qExec(&testSaleHistoryGenerator);

        TestSaleHistoryParser testSaleHistoryParser;
        QTest::qExec(&testSaleHistoryParser);

        TestSalesHistoryStreamReader testSalesHistoryStreamReader;
        QTest::qExec(&testSalesHistoryStreamReader);

        TestCsvFile testCsvFile;
        QTest::qExec(&testCsvFile);

        BenchmarkReadWriteToDb benchmarkReadWriteToDb;
        QTest::qExec(&benchmarkReadWriteToDb);

        return 0;
    }
    else
    {
        MainWindow w;
        w.show();
        return a.exec();
    }
    return 0;
}
