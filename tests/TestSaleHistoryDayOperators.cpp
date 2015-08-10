#include "TestSaleHistoryDayOperators.h"

TestSaleHistoryDayOperators::TestSaleHistoryDayOperators(QObject *parent) : QObject(parent)
{

}

void TestSaleHistoryDayOperators::TestSaleHistoryDayOperatorEqual()
{
    QFETCH(QString, leftStorage);
    QFETCH(QString, leftProduct);
    QFETCH(QDate, leftDate);
    QFETCH(double, leftSold);
    QFETCH(double, leftRest);

    QFETCH(QString, rightStorage);
    QFETCH(QString, rightProduct);
    QFETCH(QDate, rightDate);
    QFETCH(double, rightSold);
    QFETCH(double, rightRest);

    QFETCH(bool, expResult);

    SaleHistoryDay left(Item(leftStorage, leftProduct), leftDate);
    left.setSold(leftSold);
    left.setRest(leftRest);

    SaleHistoryDay right(Item(rightStorage, rightProduct), rightDate);
    right.setSold(rightSold);
    right.setRest(rightRest);

    bool actResult = (left == right);
    QCOMPARE(actResult, expResult);
}

void TestSaleHistoryDayOperators::TestSaleHistoryDayOperatorEqual_data()
{
    QTest::addColumn<QString>("leftStorage");
    QTest::addColumn<QString>("leftProduct");
    QTest::addColumn<QDate>("leftDate");
    QTest::addColumn<double>("leftSold");
    QTest::addColumn<double>("leftRest");

    QTest::addColumn<QString>("rightStorage");
    QTest::addColumn<QString>("rightProduct");
    QTest::addColumn<QDate>("rightDate");
    QTest::addColumn<double>("rightSold");
    QTest::addColumn<double>("rightRest");

    QTest::addColumn<bool>("expResult");

    QTest::newRow("equal") << QString("storage1")
                           << QString("product1")
                           << QDate(2015, 8, 8)
                           << 100.6
                           << 50.0
                           << QString("storage1")
                           << QString("product1")
                           << QDate(2015, 8, 8)
                           << 100.6
                           << 50.0
                           << true;

    QTest::newRow("not equal storage") << QString("storage1")
                                       << QString("product1")
                                       << QDate(2015, 8, 8)
                                       << 100.6
                                       << 50.0
                                       << QString("storage10")
                                       << QString("product1")
                                       << QDate(2015, 8, 8)
                                       << 100.6
                                       << 50.0
                                       << false;

    QTest::newRow("not equal product") << QString("storage1")
                                       << QString("product1")
                                       << QDate(2015, 8, 8)
                                       << 100.6
                                       << 50.0
                                       << QString("storage1")
                                       << QString("prod1")
                                       << QDate(2015, 8, 8)
                                       << 100.6
                                       << 50.0
                                       << false;

    QTest::newRow("not equal date") << QString("storage1")
                                    << QString("product1")
                                    << QDate(2015, 8, 18)
                                    << 100.6
                                    << 50.0
                                    << QString("storage1")
                                    << QString("product1")
                                    << QDate(2015, 8, 8)
                                    << 100.6
                                    << 50.0
                                    << false;

    QTest::newRow("not equal sold") << QString("storage1")
                                    << QString("product1")
                                    << QDate(2015, 8, 8)
                                    << 100.6
                                    << 50.0
                                    << QString("storage1")
                                    << QString("product1")
                                    << QDate(2015, 8, 8)
                                    << 10.6
                                    << 50.0
                                    << false;

    QTest::newRow("not equal rest") << QString("storage1")
                                    << QString("product1")
                                    << QDate(2015, 8, 8)
                                    << 100.6
                                    << 50.0
                                    << QString("storage1")
                                    << QString("product1")
                                    << QDate(2015, 8, 8)
                                    << 100.6
                                    << 50.5
                                    << false;
}

void TestSaleHistoryDayOperators::TestSaleHistoryDayOperatorNotEqual()
{
    QFETCH(QString, leftStorage);
    QFETCH(QString, leftProduct);
    QFETCH(QDate, leftDate);
    QFETCH(double, leftSold);
    QFETCH(double, leftRest);

    QFETCH(QString, rightStorage);
    QFETCH(QString, rightProduct);
    QFETCH(QDate, rightDate);
    QFETCH(double, rightSold);
    QFETCH(double, rightRest);

    QFETCH(bool, expResult);

    SaleHistoryDay left(Item(leftStorage, leftProduct), leftDate);
    left.setSold(leftSold);
    left.setRest(leftRest);

    SaleHistoryDay right(Item(rightStorage, rightProduct), rightDate);
    right.setSold(rightSold);
    right.setRest(rightRest);

    bool actResult = (left != right);
    QCOMPARE(actResult, expResult);
}

void TestSaleHistoryDayOperators::TestSaleHistoryDayOperatorNotEqual_data()
{
    QTest::addColumn<QString>("leftStorage");
    QTest::addColumn<QString>("leftProduct");
    QTest::addColumn<QDate>("leftDate");
    QTest::addColumn<double>("leftSold");
    QTest::addColumn<double>("leftRest");

    QTest::addColumn<QString>("rightStorage");
    QTest::addColumn<QString>("rightProduct");
    QTest::addColumn<QDate>("rightDate");
    QTest::addColumn<double>("rightSold");
    QTest::addColumn<double>("rightRest");

    QTest::addColumn<bool>("expResult");

    QTest::newRow("equal") << QString("storage1")
                           << QString("product1")
                           << QDate(2015, 8, 8)
                           << 100.6
                           << 50.0
                           << QString("storage1")
                           << QString("product1")
                           << QDate(2015, 8, 8)
                           << 100.6
                           << 50.0
                           << false;

    QTest::newRow("not equal storage") << QString("storage1")
                                       << QString("product1")
                                       << QDate(2015, 8, 8)
                                       << 100.6
                                       << 50.0
                                       << QString("storage10")
                                       << QString("product1")
                                       << QDate(2015, 8, 8)
                                       << 100.6
                                       << 50.0
                                       << true;

    QTest::newRow("not equal product") << QString("storage1")
                                       << QString("product1")
                                       << QDate(2015, 8, 8)
                                       << 100.6
                                       << 50.0
                                       << QString("storage1")
                                       << QString("prod1")
                                       << QDate(2015, 8, 8)
                                       << 100.6
                                       << 50.0
                                       << true;

    QTest::newRow("not equal date") << QString("storage1")
                                    << QString("product1")
                                    << QDate(2015, 8, 18)
                                    << 100.6
                                    << 50.0
                                    << QString("storage1")
                                    << QString("product1")
                                    << QDate(2015, 8, 8)
                                    << 100.6
                                    << 50.0
                                    << true;

    QTest::newRow("not equal sold") << QString("storage1")
                                    << QString("product1")
                                    << QDate(2015, 8, 8)
                                    << 100.6
                                    << 50.0
                                    << QString("storage1")
                                    << QString("product1")
                                    << QDate(2015, 8, 8)
                                    << 10.6
                                    << 50.0
                                    << true;

    QTest::newRow("not equal rest") << QString("storage1")
                                    << QString("product1")
                                    << QDate(2015, 8, 8)
                                    << 100.6
                                    << 50.0
                                    << QString("storage1")
                                    << QString("product1")
                                    << QDate(2015, 8, 8)
                                    << 100.6
                                    << 50.5
                                    << true;
}

