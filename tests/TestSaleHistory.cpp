#include "TestSaleHistory.h"

TestSaleHistory::TestSaleHistory(QObject *parent) : QObject(parent)
{

}

void TestSaleHistory::testSaleHistoryByDate()
{
    QFETCH(SaleHistory, history);
    QFETCH(Date, caseDate);
    QFETCH(bool, expValid);
    QFETCH(SaleHistoryDay, expDay);

    SaleHistoryDay actDay = history.day(caseDate);

    QCOMPARE(actDay.isValid(), expValid);
    QCOMPARE(actDay, expDay);
}

void TestSaleHistory::testSaleHistoryByDate_data()
{
    QTest::addColumn<SaleHistory>("history");
    QTest::addColumn<Date>("caseDate");
    QTest::addColumn<bool>("expValid");
    QTest::addColumn<SaleHistoryDay>("expDay");

    QTest::newRow("good date") << (SaleHistory(Item(ID("storage1"), ID("product1")))
                                   << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 8), 10.0, 50.0)
                                   << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 9), 12.3, 37.7)
                                   << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 10.3, 27.4))

                               << Date(2015, 8, 9)

                               << true
                               << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 9), 12.3, 37.7);


    QTest::newRow("bad date") << (SaleHistory(Item(ID("storage1"), ID("product1")))
                                  << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 8), 10.0, 50.0)
                                  << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 9), 12.3, 37.7)
                                  << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 10.3, 27.4))

                              << Date(2015, 8, 10)

                              << true
                              << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 9), 12.3, 37.7);

    QTest::newRow("wery bad date") << (SaleHistory(Item(ID("storage1"), ID("product1")))
                                       << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 8), 10.0, 50.0)
                                       << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 9), 12.3, 37.7)
                                       << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 10.3, 27.4))

                                   << Date(2015, 8, 1)

                                   << false
                                   << SaleHistoryDay();
}

void TestSaleHistory::testSaleHistoryFromToDate()
{
    QFETCH(SaleHistory, history);
    QFETCH(Date, expFromDate);
    QFETCH(Date, expToDate);

    Date actFromDate = history.from();
    Date actToDate = history.to();

    QCOMPARE(actFromDate, expFromDate);
    QCOMPARE(actToDate, expToDate);
}

void TestSaleHistory::testSaleHistoryFromToDate_data()
{    
    QTest::addColumn<SaleHistory>("history");

    QTest::addColumn<Date>("expFromDate");
    QTest::addColumn<Date>("expToDate");

    QTest::newRow("") << (SaleHistory(Item(ID("storage1"), ID("product1")))
                          << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 14), 12.0, 50.0)
                          << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 8), 10.3, 7.7)
                          << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 11.3, 27.4)
                          << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 12.3, 37.7)
                          << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 7), 1.3, 2.4))

                      << Date(2015, 8, 7)
                      << Date(2015, 8, 14);
}

void TestSaleHistory::testSaleHistoryDays()
{    
    QFETCH(SaleHistory, history);
    QFETCH(QList<SaleHistoryDay>, expDays);

    const QList<SaleHistoryDay> actDays = history.days();

    QCOMPARE(actDays, expDays);
}

void TestSaleHistory::testSaleHistoryDays_data()
{
    QTest::addColumn<SaleHistory>("history");
    QTest::addColumn<QList<SaleHistoryDay> >("expDays");

    QTest::newRow("") << (SaleHistory(Item(ID("storage1"), ID("product1")))
                          << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 14), 12.0, 50.0)
                          << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 8), 10.3, 7.7)
                          << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 11.3, 27.4)
                          << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 12.3, 37.7)
                          << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 7), 1.3, 2.4))

                      << (QList<SaleHistoryDay>()
                          << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 7), 1.3, 2.4)
                          << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 8), 10.3, 7.7)
                          << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 11.3, 27.4)
                          << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 12.3, 37.7)
                          << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 14), 12.0, 50.0));
}
