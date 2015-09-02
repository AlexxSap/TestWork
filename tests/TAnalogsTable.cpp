#include "TAnalogsTable.h"

TAnalogsTable::TAnalogsTable(QObject *parent) : QObject(parent)
{

}

void TAnalogsTable::TestAddAnalog()
{
    QFETCH(AnalogsTable, table);
    QFETCH(Analogs, added);
    QFETCH(QSet<Analogs>, expList);
    QFETCH(bool, expIsEqual);

    table.addAnalogs(added);

    QSet<Analogs> actList = table.toList().toSet();
    const bool actIsEqual = actList == expList;

    QCOMPARE(actIsEqual, expIsEqual);
}

void TAnalogsTable::TestAddAnalog_data()
{
    QTest::addColumn<AnalogsTable>("table");
    QTest::addColumn<Analogs>("added");
    QTest::addColumn< QSet<Analogs> >("expList");
    QTest::addColumn<bool>("expIsEqual");

    QTest::newRow("add") << (AnalogsTable()
                             << (Analogs("p01")
                                 << ID("p02")
                                 << ID("p03"))
                             << (Analogs("p11")
                                 << ID("p12")))
                         << (Analogs("p21")
                             << ID("p22"))
                         << (QSet<Analogs>()
                             << (Analogs("p01")
                                 << ID("p02")
                                 << ID("p03"))
                             << (Analogs("p11")
                                 << ID("p12"))
                             << (Analogs("p21")
                                 << ID("p22")))
                         << true;


    QTest::newRow("not_equal") << (AnalogsTable()
                                   << (Analogs("p01")
                                       << ID("p02")
                                       << ID("p03"))
                                   << (Analogs("p11")
                                       << ID("p12")))
                               << (Analogs("p21")
                                   << ID("p22"))
                               << (QSet<Analogs>()
                                   << (Analogs("p01")
                                       << ID("p02")
                                       << ID("p03"))
                                   << (Analogs("p11")
                                       << ID("p1"))
                                   << (Analogs("p21")
                                       << ID("p22")))
                               << false;
}

void TAnalogsTable::TestIsInto()
{
    QFETCH(AnalogsTable, table);
    QFETCH(Analogs, analogs);
    QFETCH(bool, expIsInto);

    const bool actIsInto = table.contains(analogs);
    QCOMPARE(actIsInto, expIsInto);
}

void TAnalogsTable::TestIsInto_data()
{
    QTest::addColumn<AnalogsTable>("table");
    QTest::addColumn<Analogs>("analogs");
    QTest::addColumn<bool>("expIsInto");

    QTest::newRow("equal") << (AnalogsTable()
                               << (Analogs("p01")
                                   << ID("p02")
                                   << ID("p03"))
                               << (Analogs("p11")
                                   << ID("p12")))
                           <<(Analogs("p11")
                              << ID("p12"))
                          << true;

    QTest::newRow("equal") << (AnalogsTable()
                               << (Analogs("p01")
                                   << ID("p02")
                                   << ID("p03"))
                               << (Analogs("p11")
                                   << ID("p13")))
                           <<(Analogs("p11")
                              << ID("p12"))
                          << false;
}

void TAnalogsTable::TestAssign()
{
    QFETCH(AnalogsTable, table);

    const AnalogsTable other = table;

    bool eq = table == other;
    bool notEq = table != other;

    QVERIFY(eq);
    QVERIFY(!notEq);
}

void TAnalogsTable::TestAssign_data()
{
    QTest::addColumn<AnalogsTable>("table");
    QTest::newRow("1") << (AnalogsTable()
                          << (Analogs("p01")
                              << ID("p02")
                              << ID("p03"))
                          << (Analogs("p11")
                              << ID("p12")));

    QTest::newRow("2") << AnalogsTable();
}

void TAnalogsTable::TestAnalogsForProduct()
{
    QFETCH(AnalogsTable, table);
    QFETCH(ID, product);
    QFETCH(Analogs, expResult);

    const Analogs actResult = table.analogsForProduct(product);

    QCOMPARE(actResult, expResult);
}

void TAnalogsTable::TestAnalogsForProduct_data()
{
    QTest::addColumn<AnalogsTable>("table");
    QTest::addColumn<ID>("product");
    QTest::addColumn<Analogs>("expResult");

    QTest::newRow("empty_id") << (AnalogsTable()
                                  << (Analogs("p01")
                                      << ID("p02")
                                      << ID("p03"))
                                  << (Analogs("p11")
                                      << ID("p12"))
                                  << (Analogs("p21")
                                      << ID("p22")
                                      << ID("p23")))
                              << ID()
                              << Analogs();

    QTest::newRow("empty_table") << AnalogsTable()
                                 << ID()
                                 << Analogs();

    QTest::newRow("simple") << (AnalogsTable()
                                << (Analogs("p01")
                                    << ID("p02")
                                    << ID("p03"))
                                << (Analogs("p11")
                                    << ID("p12"))
                                << (Analogs("p21")
                                    << ID("p22")
                                    << ID("p23")))
                            << ID("p11")
                            << (Analogs("p11")
                                << ID("p12"));

    QTest::newRow("not_found") << (AnalogsTable()
                                   << (Analogs("p01")
                                       << ID("p02")
                                       << ID("p03"))
                                   << (Analogs("p11")
                                       << ID("p12"))
                                   << (Analogs("p21")
                                       << ID("p22")
                                       << ID("p23")))
                               << ID("p31")
                               << Analogs();
}
