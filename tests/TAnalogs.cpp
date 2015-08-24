#include "TAnalogs.h"

TAnalogs::TAnalogs(QObject *parent) : QObject(parent)
{

}

void TAnalogs::TestAnalogsList()
{
    QFETCH(ID, mainAnalog);
    QFETCH(QList<ID>, analogList);
    QFETCH(QList<ID>, expList);

    Analogs analog(mainAnalog);
    analog.addAnalogs(analogList);

    QList<ID> actLits = analog.analogs();

    const bool isEqual = TestUtility::compareListWithoutOrder(actLits, expList);

    if(!isEqual)
    {
        qInfo() << actLits;
        qInfo() << expList;
    }
    QVERIFY(isEqual);
}

void TAnalogs::TestAnalogsList_data()
{
    QTest::addColumn<ID>("mainAnalog");
    QTest::addColumn< QList<ID> >("analogList");
    QTest::addColumn< QList<ID> >("expList");

    QTest::newRow("simple") << ID("product1")
                            << (QList<ID>()
                                << "product2"
                                << "product3"
                                << "product5")
                            << (QList<ID>()
                                << "product1"
                                << "product2"
                                << "product3"
                                << "product5");

    QTest::newRow("empty") << ID("product1")
                           << QList<ID>()
                           << (QList<ID>() << "product1");
}

