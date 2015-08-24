#include "TAnalogs.h"

TAnalogs::TAnalogs(QObject *parent) : QObject(parent)
{

}

void TAnalogs::TestAnalogsList()
{
    QFETCH(Analogs, data);
    QFETCH(QList<ID>, expList);

    QList<ID> actLits = data.toList();

    const bool isEqual = TestUtility::compareListWithoutOrder(actLits, expList);

    QVERIFY(isEqual);
}

void TAnalogs::TestAnalogsList_data()
{
    QTest::addColumn<Analogs>("data");
    QTest::addColumn< QList<ID> >("expList");

    QTest::newRow("simple") << (Analogs("product1")
                                << "product2"
                                << "product3"
                                << "product5")
                            << (QList<ID>()
                                << "product2"
                                << "product3"
                                << "product5");

    QTest::newRow("empty") << Analogs("product1")
                           << QList<ID>();
}

void TAnalogs::TestIsAnalog()
{
    QFETCH(Analogs, data);
    QFETCH(ID, art);
    QFETCH(bool, expIsAnalog);

    bool actIsAnalog = data.contains(art);

    QCOMPARE(actIsAnalog, expIsAnalog);
}

void TAnalogs::TestIsAnalog_data()
{
    QTest::addColumn<Analogs>("data");
    QTest::addColumn<ID>("art");
    QTest::addColumn<bool>("expIsAnalog");

    QTest::newRow("yes") << (Analogs("product1")
                             << "product1"
                             << "product2")
                         << ID("product2")
                         << true;

    QTest::newRow("no") << (Analogs("product1")
                            << "product1"
                            << "product2")
                        << ID("product3")
                        << false;
}

void TAnalogs::TestAnalogsAssign()
{
    QFETCH(Analogs, data);

    const Analogs other = data;

    qInfo() << other;
    qInfo() << data;

    bool eq = data == other;
    bool notEq = data != other;

    QVERIFY(eq);
    QVERIFY(!notEq);
}

void TAnalogs::TestAnalogsAssign_data()
{
    QTest::addColumn<Analogs>("data");

    QTest::newRow("") << (Analogs("product1")
                          << "product1"
                          << "product2");

}

