#include "TestSaleHistoryDayOperators.h"

TestSaleHistoryDayOperators::TestSaleHistoryDayOperators(QObject *parent) : QObject(parent)
{

}

void TestSaleHistoryDayOperators::TestSaleHistoryDayOperatorEqual()
{
    QFETCH(SaleHistoryDay, left);
    QFETCH(SaleHistoryDay, right);
    QFETCH(bool, expResult);

    const bool actEqual = (left == right);
    const bool actNotEqual = (left != right);
    QCOMPARE(actEqual, expResult);
    QCOMPARE(actNotEqual, !expResult);
}

void TestSaleHistoryDayOperators::TestSaleHistoryDayOperatorEqual_data()
{
    QTest::addColumn<SaleHistoryDay>("left");
    QTest::addColumn<SaleHistoryDay>("right");

    QTest::addColumn<bool>("expResult");

    QTest::newRow("equal")  << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 8), 100.6, 50.0)
                            << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 8), 100.6, 50.0)
                            << true;

    QTest::newRow("not equal storage") << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 8), 100.6, 50.0)
                                       << SaleHistoryDay(Item(ID("storage10"), ID("product1")), Date(2015, 8, 8), 100.6, 50.0)
                                       << false;

    QTest::newRow("not equal product") << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 8), 100.6, 50.0)
                                       << SaleHistoryDay(Item(ID("storage1"), ID("prod1")), Date(2015, 8, 8), 100.6, 50.0)
                                       << false;

    QTest::newRow("not equal date") << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 18), 100.6, 50.0)
                                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 8), 100.6, 50.0)
                                    << false;

    QTest::newRow("not equal sold") << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 8), 10.6, 50.0)
                                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 8), 100.6, 50.0)
                                    << false;

    QTest::newRow("not equal rest") << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 8), 100.6, 50.0)
                                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 8), 100.6, 5.0)
                                    << false;
}
