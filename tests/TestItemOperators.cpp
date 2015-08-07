#include "TestItemOperators.h"

TestItemOperators::TestItemOperators(QObject *parent) : QObject(parent)
{

}

void TestItemOperators::testItemOperatorsAssign()
{
    QFETCH(QString, realStorage);
    QFETCH(QString, realProduct);

    Item real(realStorage, realProduct);

    Item assigned = real;
    bool actResult = (real == assigned);
    QVERIFY(actResult);
}

void TestItemOperators::testItemOperatorsAssign_data()
{
    QTest::addColumn<QString>("realStorage");
    QTest::addColumn<QString>("realProduct");

    QTest::newRow("Assignment") << QString("storage1")
                                << QString("product1");
}

void TestItemOperators::testItemOperatorsIsEqual()
{
    QFETCH(QString, leftStorage);
    QFETCH(QString, leftProduct);
    QFETCH(QString, rightStorage);
    QFETCH(QString, rightProduct);
    QFETCH(bool, expResult);

    Item left(leftStorage, leftProduct);
    Item right(rightStorage, rightProduct);

    bool actResult = (left == right);
    QCOMPARE(expResult, actResult);
}

void TestItemOperators::testItemOperatorsIsEqual_data()
{
    QTest::addColumn<QString>("leftStorage");
    QTest::addColumn<QString>("leftProduct");
    QTest::addColumn<QString>("rightStorage");
    QTest::addColumn<QString>("rightProduct");
    QTest::addColumn<bool>("expResult");

    QTest::newRow("equal") << QString("storage1")
                           << QString("product1")
                           << QString("storage1")
                           << QString("product1")
                           << true;

    QTest::newRow("not equal") << QString("storage1")
                               << QString("product1")
                               << QString("storage2")
                               << QString("product1")
                               << false;
}

void TestItemOperators::testItemOperatorsIsNotEqual()
{
    QFETCH(QString, leftStorage);
    QFETCH(QString, leftProduct);
    QFETCH(QString, rightStorage);
    QFETCH(QString, rightProduct);
    QFETCH(bool, expResult);

    Item left(leftStorage, leftProduct);
    Item right(rightStorage, rightProduct);

    bool actResult = (left != right);
    QCOMPARE(expResult, actResult);
}

void TestItemOperators::testItemOperatorsIsNotEqual_data()
{
    QTest::addColumn<QString>("leftStorage");
    QTest::addColumn<QString>("leftProduct");
    QTest::addColumn<QString>("rightStorage");
    QTest::addColumn<QString>("rightProduct");
    QTest::addColumn<bool>("expResult");

    QTest::newRow("equal") << QString("storage1")
                           << QString("product1")
                           << QString("storage1")
                           << QString("product1")
                           << false;

    QTest::newRow("not equal") << QString("storage1")
                               << QString("product1")
                               << QString("storage2")
                               << QString("product1")
                               << true;
}
