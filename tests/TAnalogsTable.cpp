#include "TAnalogsTable.h"

TAnalogsTable::TAnalogsTable(QObject *parent) : QObject(parent)
{

}

void TAnalogsTable::TestAddAnalog()
{
    QFETCH(AnalogsTable, table);
    QFETCH(Analogs, added);
    QFETCH(QList<Analogs>, expList);
    QFETCH(bool, expIsEqual);

    table.addAnalogs(added);

    QList<Analogs> actList = table.toList();

    const bool actIsEqual = TestUtility::compareListWithoutOrder(actList, expList);
    QCOMPARE(actIsEqual, expIsEqual);

}

void TAnalogsTable::TestAddAnalog_data()
{
    QTest::addColumn<AnalogsTable>("table");
    QTest::addColumn<Analogs>("added");
    QTest::addColumn<QList<Analogs> >("expList");
    QTest::addColumn<bool>("expIsEqual");

    QTest::newRow("add") << (AnalogsTable()
                             << (Analogs("p01")
                                 << ID("p02")
                                 << ID("p03"))
                             << (Analogs("p11")
                                 << ID("p12")))
                         << (Analogs("p21")
                             << ID("p22"))
                         << (QList<Analogs>()
                             << (Analogs("p01")
                                 << ID("p02")
                                 << ID("p03"))
                             << (Analogs("p11")
                                 << ID("p12"))
                             << (Analogs("p21")
                                 << ID("p22")))
                         << true;


    QTest::newRow("not equal") << (AnalogsTable()
                                   << (Analogs("p01")
                                       << ID("p02")
                                       << ID("p03"))
                                   << (Analogs("p11")
                                       << ID("p12")))
                               << (Analogs("p21")
                                   << ID("p22"))
                               << (QList<Analogs>()
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

    qInfo() << table;
    qInfo() << other;

    bool eq = table == other;
    bool notEq = table != other;

    QVERIFY(eq);
    QVERIFY(!notEq);
}

void TAnalogsTable::TestAssign_data()
{
    QTest::addColumn<AnalogsTable>("table");
    QTest::newRow("") << (AnalogsTable()
                          << (Analogs("p01")
                              << ID("p02")
                              << ID("p03"))
                          << (Analogs("p11")
                              << ID("p12")));
}

