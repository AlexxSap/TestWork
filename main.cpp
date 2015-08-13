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

///notes конечные цели:
/// 1. уметь генерировать историю продаж на заданый временной
/// период для заданого числа артикулов и складов
/// 2. уметь читать "подряд" несколько историй продаж для заданного набора товаров
/// 3. уметь замерять за какое время записываются и считываются определённые
/// объёмы историй продаж

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
