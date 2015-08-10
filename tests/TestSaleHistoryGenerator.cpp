#include "TestSaleHistoryGenerator.h"

TestSaleHistoryGenerator::TestSaleHistoryGenerator(QObject *parent) : QObject(parent)
{

}

bool TestSaleHistoryGenerator::compareData(const QList<SaleHistoryDay> &list,
                                           int maxVal)
{
    foreach (SaleHistoryDay day, list)
    {
        if(day.sold() < 0
                || day.sold() > maxVal
                || day.rest() < 0
                || day.rest() > maxVal)
        {
            return false;
        }
    }
    return true;
}

void TestSaleHistoryGenerator::testSaleHistoryGenerator()
{
    QFETCH(QDate, fromDate);
    QFETCH(QDate, toDate);
    QFETCH(int, storageNum);
    QFETCH(int, productNum);
    QFETCH(int, maxVal);

    SaleHistoryGenerator gen;
    gen.setMaxValue(maxVal);
    QList<SaleHistoryDay> list = gen.generateHistory(fromDate,
                                                     toDate,
                                                     storageNum,
                                                     productNum);

    int actNum = storageNum * productNum * (fromDate.daysTo(toDate) + 1);
    bool comp = compareData(list, maxVal);
    QVERIFY(list.count() == actNum);
    QVERIFY(comp);
}

void TestSaleHistoryGenerator::testSaleHistoryGenerator_data()
{
    QTest::addColumn<QDate>("fromDate");
    QTest::addColumn<QDate>("toDate");
    QTest::addColumn<int>("storageNum");
    QTest::addColumn<int>("productNum");
    QTest::addColumn<int>("maxVal");

    QTest::newRow("gen test 1") << QDate(2015, 3, 1)
                                << QDate(2015, 3, 10)
                                << 3
                                << 5
                                << 10000;

    QTest::newRow("gen test 2") << QDate(2015, 2, 20)
                                << QDate(2015, 3, 4)
                                << 3
                                << 15
                                << 1000;
}

