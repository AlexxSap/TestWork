#include "TestItemOperators.h"

TestItemOperators::TestItemOperators(QObject *parent) : QObject(parent)
{

}

void TestItemOperators::testItemOperatorsAssign()
{
    QFETCH(Item, real);

    Item assigned = real;
    bool actResult = (real == assigned);
    QVERIFY(actResult);
}

void TestItemOperators::testItemOperatorsAssign_data()
{
    QTest::addColumn<Item>("real");

    QTest::newRow("Assignment") << Item(ID("storage1"), ID("product1"));
}

void TestItemOperators::testItemOperatorsIsEqual()
{
    QFETCH(Item, left);
    QFETCH(Item, right);
    QFETCH(bool, expResult);

    const bool actEqual = (left == right);
    const bool actNotEqual = (left != right);
    QCOMPARE(actEqual, expResult);
    QCOMPARE(!actNotEqual, expResult);
}

void TestItemOperators::testItemOperatorsIsEqual_data()
{
    QTest::addColumn<Item>("left");
    QTest::addColumn<Item>("right");
    QTest::addColumn<bool>("expResult");

    QTest::newRow("equal") << Item(ID("storage1"), ID("product1"))
                           << Item(ID("storage1"), ID("product1"))
                           << true;

    QTest::newRow("not equal storage") << Item(ID("storage1"), ID("product1"))
                                       << Item(ID("storage2"), ID("product1"))
                                       << false;

    QTest::newRow("not equal product") << Item(ID("storage1"), ID("product1"))
                                       << Item(ID("storage2"), ID("prod1"))
                                       << false;
}
