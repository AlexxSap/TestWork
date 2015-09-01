#include "TAnalogs.h"

TAnalogs::TAnalogs(QObject *parent) : QObject(parent)
{

}

void TAnalogs::TestAnalogsList()
{
    QFETCH(Analogs, data);
    QFETCH(QSet<ID>, expList);

    QSet<ID> actLits = data.toList().toSet();

    QCOMPARE(actLits, expList);
}

void TAnalogs::TestAnalogsList_data()
{
    QTest::addColumn<Analogs>("data");
    QTest::addColumn< QSet<ID> >("expList");

    QTest::newRow("simple") << (Analogs("product1")
                                << "product2"
                                << "product3"
                                << "product5")
                            << (QSet<ID>()
                                << "product1"
                                << "product2"
                                << "product3"
                                << "product5");

    QTest::newRow("empty") << Analogs("product1")
                           << (QSet<ID>()
                               << "product1");
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
                             << "product3"
                             << "product2")
                         << ID("product2")
                         << true;

    QTest::newRow("yes2") << (Analogs("product1")
                             << "product3"
                             << "product2")
                         << ID("product1")
                         << true;

    QTest::newRow("no") << (Analogs("product1")
                            << "product4"
                            << "product2")
                        << ID("product3")
                        << false;

    QTest::newRow("one") << Analogs("product3")
                        << ID("product3")
                        << true;

    QTest::newRow("empty") << Analogs()
                        << ID("product3")
                        << false;
}

void TAnalogs::TestAnalogsAssign()
{
    QFETCH(Analogs, data);

    const Analogs other = data;

    bool eq = data == other;
    bool notEq = data != other;

    QVERIFY(eq);
    QVERIFY(!notEq);
}

void TAnalogs::TestAnalogsAssign_data()
{
    QTest::addColumn<Analogs>("data");

    QTest::newRow("first") << (Analogs("product1")
                          << "product1"
                          << "product2");

    QTest::newRow("second") << Analogs("product1");

}

