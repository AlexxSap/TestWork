#include "TestSaleHistoryGenerator.h"

TestSaleHistoryGenerator::TestSaleHistoryGenerator(QObject *parent) : QObject(parent)
{

}

///notes что бы выбрать, что нужно обязательно тестировать, а что нет,
/// можно задаться вопросом
/// "а что будет, если эта компонента будет работать неверно
///  и как быстро это будет замечено".
/// что будет, если эта довольно сложная функция(цикл и четыре условия)
/// будет работать неверно и как быстро это будет замечено?
bool TestSaleHistoryGenerator::compareData(const QList<SaleHistoryDay> &list,
                                           const int &maxVal)
{
    foreach (const SaleHistoryDay &day, list)
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


void TestSaleHistoryGenerator::testCompareData()
{

}

void TestSaleHistoryGenerator::testCompareData_data()
{
    QTest::addColumn< QList<SaleHistoryDay> >("list");
    QTest::addColumn<int>("maxVal");
    QTest::addColumn<bool>("expRes");

    QTest::newRow("yes") << (QList<SaleHistoryDay>()
                             << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 14), 12.0, 50.0)
                             << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 8), 10.3, 7.7)
                             << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 11.3, 27.4)
                             << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 12.3, 37.7)
                             << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 7), 1.3, 2.4))

                         << 100
                         << true;

    QTest::newRow("no 1") << (QList<SaleHistoryDay>()
                              << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 14), -12.0, 50.0)
                              << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 8), 10.3, 7.7)
                              << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 11.3, 27.4)
                              << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 12.3, 37.7)
                              << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 7), 1.3, 2.4))

                          << 100
                          << false;

    QTest::newRow("no 2") << (QList<SaleHistoryDay>()
                              << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 14), 12.0, 50.0)
                              << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 8), 10.3, 7.7)
                              << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 111.3, 27.4)
                              << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 12.3, 37.7)
                              << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 7), 1.3, 2.4))

                          << 100
                          << false;

    QTest::newRow("no 3") << (QList<SaleHistoryDay>()
                              << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 14), 12.0, 50.0)
                              << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 8), 10.3, 7.7)
                              << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 11.3, 27.4)
                              << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 12.3, 37.7)
                              << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 7), 1.3, -2.4))

                          << 100
                          << false;

    QTest::newRow("no 4") << (QList<SaleHistoryDay>()
                              << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 14), 12.0, 50.0)
                              << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 8), 10.3, 7.7)
                              << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 11.3, 127.4)
                              << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 12.3, 37.7)
                              << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 7), 1.3, 142.4))

                          << 100
                          << false;
}

void TestSaleHistoryGenerator::testSaleHistoryGenerator()
{
    QFETCH(QDate, fromDate);
    QFETCH(QDate, toDate);
    QFETCH(int, storageNum);
    QFETCH(int, productNum);
    QFETCH(int, expNumber);
    QFETCH(int, maxVal);

    const QList<SaleHistoryDay> list = SaleHistoryGenerator::generateHistory(fromDate,
                                                                             toDate,
                                                                             storageNum,
                                                                             productNum,
                                                                             maxVal);

    bool comp = compareData(list, maxVal);

    QCOMPARE(list.count(), expNumber);
    QVERIFY(comp);
}

void TestSaleHistoryGenerator::testSaleHistoryGenerator_data()
{
    QTest::addColumn<QDate>("fromDate");
    QTest::addColumn<QDate>("toDate");
    QTest::addColumn<int>("storageNum");
    QTest::addColumn<int>("productNum");
    QTest::addColumn<int>("expNumber");
    QTest::addColumn<int>("maxVal");

    QTest::newRow("gen test 1") << QDate(2015, 3, 1)
                                << QDate(2015, 3, 10)
                                << 3
                                << 5
                                << 3 * 5 * 10
                                << 10000;

    QTest::newRow("gen test 2") << QDate(2015, 2, 20)
                                << QDate(2015, 3, 4)
                                << 3
                                << 15
                                << 3 * 15 * 13
                                << 1000;

    QTest::newRow("wronge gen test") << QDate(2015, 3, 20)
                                     << QDate(2015, 3, 4)
                                     << 3
                                     << 15
                                     << 0
                                     << 1000;

    QTest::newRow("invalid date") << QDate(2015, 2, 20)
                                     << QDate(2015, 2, 29)
                                     << 0
                                     << 0
                                     << 0
                                     << 0;
}

