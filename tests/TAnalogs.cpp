#include "TAnalogs.h"

TAnalogs::TAnalogs(QObject *parent) : QObject(parent)
{

}

void TAnalogs::TestAnalogsList()
{
    QFETCH(Analogs, data);
    ///notes если в тестах используем QList, то это скорее всего означает, что нам важен порядок либо возможность иметь повторяющиейся элементы
    /// если ни то ни то не важно, то используем QSet.
    ///  в данном случае это бы заодно избавило от использования TestUtility::compareListWithoutOrder(довольно сложной функции, которая сама по себе требует тестирования)
    QFETCH(QList<ID>, expList);

    QList<ID> actLits = data.toList();

    qInfo() << expList;
    qInfo() << actLits;
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
                                << "product1"
                                << "product2"
                                << "product3"
                                << "product5");

    QTest::newRow("empty") << Analogs("product1")
                           << (QList<ID>()
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

///notes в тестах обязательно должны присутвовать краевые значения. тут - это аналоги из одного товара
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

