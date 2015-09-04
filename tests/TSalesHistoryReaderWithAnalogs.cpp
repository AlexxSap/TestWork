#include "TSalesHistoryReaderWithAnalogs.h"

TSalesHistoryReaderWithAnalogs::TSalesHistoryReaderWithAnalogs(QObject *parent) : QObject(parent)
{

}

void TSalesHistoryReaderWithAnalogs::TestSalesHistoryReaderWithAnalogs()
{
    QFETCH(QList<SaleHistoryDay>, history);
    QFETCH(AnalogsTable, analogsTable);
    QFETCH(QList<Item>, caseItems);
    QFETCH(Date, caseDateFrom);
    QFETCH(Date, caseDateTo);
    QFETCH(QSet<SaleHistory>, expResult);

    const QString dbName(QString(QTest::currentDataTag()) + "tdb");
    DataBaseInfo info;
    info.setDataBaseName(dbName);
    info.setUserName("root");
    info.setPassword("1234");

    if(!DataBase::remDataBase(info))
    {
        QFAIL("cannot remove test-db in begin of test");
    }

    bool result = false;
    {
        SaleHistoryWriter writer(info);
        result = writer.write(history);
    }
    if(!result)
    {
        DataBase::remDataBase(info);
        QFAIL("cannot write history to db");
    }

    {
        AnalogsWriter writer(info);
        result = writer.write(analogsTable);
    }
    if(!result)
    {
        DataBase::remDataBase(info);
        QFAIL("cannot write analogs to db");
    }

    QList<SaleHistory> actResult;
    {
        SalesHistoryStreamReader reader(caseItems, info);
        bool isOpen = reader.open(caseDateFrom, caseDateTo);
        if(isOpen)
        {
            do
            {
                const SaleHistory curHistory = reader.current();
                //            qInfo() << curHistory;
                actResult.append(curHistory);
            }while (reader.next());
        }
    }

    if(!DataBase::remDataBase(info))
    {
        QFAIL("cannot remove test-db in end of test");
    }
    QCOMPARE(actResult.toSet(), expResult);
}

void TSalesHistoryReaderWithAnalogs::TestSalesHistoryReaderWithAnalogs_data()
{
    QTest::addColumn< QList<SaleHistoryDay> >("history");
    QTest::addColumn<AnalogsTable>("analogsTable");

    QTest::addColumn< QList<Item> >("caseItems");
    QTest::addColumn<Date>("caseDateFrom");
    QTest::addColumn<Date>("caseDateTo");

    QTest::addColumn< QSet<SaleHistory> >("expResult");

    QTest::newRow("empty_data")
            << QList<SaleHistoryDay>()
            << (AnalogsTable() << (Analogs("p01")
                                   << ID("p1")
                                   << ID("p2")))

            << (QList<Item>() << Item(ID("s1"), ID("p1"))
                << Item(ID("s1"), ID("p2")))
            << Date(2015, 8, 10)
            << Date(2015, 8, 13)

            << (QSet<SaleHistory>()
                << (SaleHistory(Item(ID("s1"), ID("p01")))
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 10), 0.0, 0.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 11), 0.0, 0.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 12), 0.0, 0.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 13), 0.0, 0.0)));

    QTest::newRow("simple")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 10), 1.0, 2.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 12), 2.0, 4.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p2")), Date(2015, 8, 10), 4.0, 0.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p2")), Date(2015, 8, 11), 1.0, 3.0))
            << (AnalogsTable() << (Analogs("p01")
                                   << ID("p1")
                                   << ID("p2")))

            << (QList<Item>() << Item(ID("s1"), ID("p1"))
                << Item(ID("s1"), ID("p2")))
            << Date(2015, 8, 10)
            << Date(2015, 8, 13)

            << (QSet<SaleHistory>()
                << (SaleHistory(Item(ID("s1"), ID("p01")))
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 10), 5.0, 2.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 11), 1.0, 5.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 12), 2.0, 7.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 13), 0.0, 7.0)));

    QTest::newRow("not_simple_sorting")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("s1"), ID("p5")), Date(2015, 8, 10), 1.0, 2.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p5")), Date(2015, 8, 12), 2.0, 4.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p2")), Date(2015, 8, 10), 4.0, 0.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p2")), Date(2015, 8, 11), 1.0, 3.0))
            << (AnalogsTable() << (Analogs("p4")
                                   << ID("p1")
                                   << ID("p5")
                                   << ID("p2")))

            << (QList<Item>() << Item(ID("s1"), ID("p1")))
            << Date()
            << Date()

            << (QSet<SaleHistory>()
                << (SaleHistory(Item(ID("s1"), ID("p4")))
                    << SaleHistoryDay(Item(ID("s1"), ID("p4")), Date(2015, 8, 10), 5.0, 2.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p4")), Date(2015, 8, 11), 1.0, 5.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p4")), Date(2015, 8, 12), 2.0, 7.0)));

    QTest::newRow("not_simple_sorting_2")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("s1"), ID("p5")), Date(2015, 8, 10), 1.0, 2.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p5")), Date(2015, 8, 12), 2.0, 4.0)
                << SaleHistoryDay(Item(ID("s2"), ID("p2")), Date(2015, 8, 10), 4.0, 0.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p2")), Date(2015, 8, 11), 1.0, 3.0))
            << (AnalogsTable() << (Analogs("p4")
                                   << ID("p1")
                                   << ID("p5")
                                   << ID("p2")))

            << (QList<Item>() << Item(ID("s1"), ID("p1")))
            << Date()
            << Date()

            << (QSet<SaleHistory>()
                << (SaleHistory(Item(ID("s1"), ID("p4")))
                    << SaleHistoryDay(Item(ID("s1"), ID("p4")), Date(2015, 8, 10), 1.0, 2.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p4")), Date(2015, 8, 11), 1.0, 5.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p4")), Date(2015, 8, 12), 2.0, 7.0)));


    QTest::newRow("by_analog")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 10), 1.0, 2.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 12), 2.0, 4.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p2")), Date(2015, 8, 10), 4.0, 0.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p2")), Date(2015, 8, 11), 1.0, 3.0))
            << (AnalogsTable() << (Analogs("p01")
                                   << ID("p1")
                                   << ID("p5")))

            << (QList<Item>() << Item(ID("s1"), ID("p5")))
            << Date(2015, 8, 10)
            << Date(2015, 8, 13)

            << (QSet<SaleHistory>()
                << (SaleHistory(Item(ID("s1"), ID("p01")))
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 10), 1.0, 2.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 11), 0.0, 2.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 12), 2.0, 4.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 13), 0.0, 4.0)));


    QTest::newRow("by_analog_on_diff_storages")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 10), 1.0, 2.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 12), 2.0, 4.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p2")), Date(2015, 8, 10), 4.0, 0.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p2")), Date(2015, 8, 11), 1.0, 3.0)

                << SaleHistoryDay(Item(ID("s2"), ID("p1")), Date(2015, 8, 10), 10.0, 20.0)
                << SaleHistoryDay(Item(ID("s2"), ID("p1")), Date(2015, 8, 12), 20.0, 40.0)
                << SaleHistoryDay(Item(ID("s2"), ID("p2")), Date(2015, 8, 10), 40.0, 5.0)
                << SaleHistoryDay(Item(ID("s2"), ID("p4")), Date(2015, 8, 11), 1.0, 3.0))
            << (AnalogsTable() << (Analogs("p01")
                                   << ID("p1")
                                   << ID("p2")
                                   << ID("p5")))

            << (QList<Item>()
                << Item(ID("s1"), ID("p5"))
                << Item(ID("s2"), ID("p5")))
            << Date(2015, 8, 10)
            << Date(2015, 8, 13)

            << (QSet<SaleHistory>()
                << (SaleHistory(Item(ID("s1"), ID("p01")))
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 10), 5.0, 2.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 11), 1.0, 5.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 12), 2.0, 7.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 13), 0.0, 7.0))
                << (SaleHistory(Item(ID("s2"), ID("p01")))
                    << SaleHistoryDay(Item(ID("s2"), ID("p01")), Date(2015, 8, 10), 50.0, 25.0)
                    << SaleHistoryDay(Item(ID("s2"), ID("p01")), Date(2015, 8, 11), 0.0, 25.0)
                    << SaleHistoryDay(Item(ID("s2"), ID("p01")), Date(2015, 8, 12), 20.0, 45.0)
                    << SaleHistoryDay(Item(ID("s2"), ID("p01")), Date(2015, 8, 13), 0.0, 45.0)));

    QTest::newRow("empty_item")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 10), 1.0, 2.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 12), 2.0, 4.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p2")), Date(2015, 8, 10), 4.0, 0.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p2")), Date(2015, 8, 11), 1.0, 3.0))
            << (AnalogsTable() << (Analogs("p01")
                                   << ID("p1")
                                   << ID("p5")))

            << QList<Item>()
            << Date(2015, 8, 10)
            << Date(2015, 8, 13)

            << QSet<SaleHistory>();


    QTest::newRow("different_storage")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 10), 1.0, 2.0)
                << SaleHistoryDay(Item(ID("s2"), ID("p1")), Date(2015, 8, 12), 2.0, 4.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p2")), Date(2015, 8, 10), 4.0, 0.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p2")), Date(2015, 8, 11), 1.0, 3.0))
            << (AnalogsTable() << (Analogs("p01")
                                   << ID("p1")
                                   << ID("p2")))

            << (QList<Item>() << Item(ID("s1"), ID("p1"))
                << Item(ID("s2"), ID("p1"))
                << Item(ID("s1"), ID("p2")))
            << Date(2015, 8, 10)
            << Date(2015, 8, 13)

            << (QSet<SaleHistory>()
                << (SaleHistory(Item(ID("s1"), ID("p01")))
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 10), 5.0, 2.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 11), 1.0, 5.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 12), 0.0, 5.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 13), 0.0, 5.0))
                << (SaleHistory(Item(ID("s2"), ID("p01")))
                    << SaleHistoryDay(Item(ID("s2"), ID("p01")), Date(2015, 8, 10), 0.0, 0.0)
                    << SaleHistoryDay(Item(ID("s2"), ID("p01")), Date(2015, 8, 11), 0.0, 0.0)
                    << SaleHistoryDay(Item(ID("s2"), ID("p01")), Date(2015, 8, 12), 2.0, 4.0)
                    << SaleHistoryDay(Item(ID("s2"), ID("p01")), Date(2015, 8, 13), 0.0, 4.0)));

    QTest::newRow("different_storage_2")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 10), 1.0, 2.0)
                << SaleHistoryDay(Item(ID("s2"), ID("p1")), Date(2015, 8, 12), 2.0, 4.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 11), 3.0, 5.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p2")), Date(2015, 8, 10), 4.0, 0.0)
                << SaleHistoryDay(Item(ID("s2"), ID("p2")), Date(2015, 8, 11), 1.0, 3.0)
                << SaleHistoryDay(Item(ID("s2"), ID("p2")), Date(2015, 8, 13), 3.0, 1.0))
            << (AnalogsTable() << (Analogs("p01")
                                   << ID("p1")
                                   << ID("p2")
                                   << ID("p3")))

            << (QList<Item>() << Item(ID("s1"), ID("p1"))
                << Item(ID("s2"), ID("p1"))
                << Item(ID("s1"), ID("p2"))
                << Item(ID("s2"), ID("p2")))
            << Date(2015, 8, 10)
            << Date(2015, 8, 13)

            << (QSet<SaleHistory>()
                << (SaleHistory(Item(ID("s1"), ID("p01")))
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 10), 5.0, 2.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 11), 3.0, 5.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 12), 0.0, 5.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 13), 0.0, 5.0))
                << (SaleHistory(Item(ID("s2"), ID("p01")))
                    << SaleHistoryDay(Item(ID("s2"), ID("p01")), Date(2015, 8, 10), 0.0, 0.0)
                    << SaleHistoryDay(Item(ID("s2"), ID("p01")), Date(2015, 8, 11), 1.0, 3.0)
                    << SaleHistoryDay(Item(ID("s2"), ID("p01")), Date(2015, 8, 12), 2.0, 7.0)
                    << SaleHistoryDay(Item(ID("s2"), ID("p01")), Date(2015, 8, 13), 3.0, 5.0)));

    QTest::newRow("different_analogs")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 10), 1.0, 2.0)
                << SaleHistoryDay(Item(ID("s2"), ID("p1")), Date(2015, 8, 12), 2.0, 4.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p2")), Date(2015, 8, 10), 4.0, 0.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p3")), Date(2015, 8, 12), 2.0, 4.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p4")), Date(2015, 8, 10), 4.0, 1.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p2")), Date(2015, 8, 11), 1.0, 3.0))
            << (AnalogsTable()
                << (Analogs("p1")
                    << ID("p2"))
                << (Analogs("p03")
                    << ID("p4")
                    << ID("p3")))

            << (QList<Item>() << Item(ID("s1"), ID("p1"))
                << Item(ID("s2"), ID("p1"))
                << Item(ID("s1"), ID("p2"))
                << Item(ID("s1"), ID("p3"))
                << Item(ID("s1"), ID("p4")))
            << Date(2015, 8, 10)
            << Date(2015, 8, 13)

            << (QSet<SaleHistory>()
                << (SaleHistory(Item(ID("s1"), ID("p1")))
                    << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 10), 5.0, 2.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 11), 1.0, 5.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 12), 0.0, 5.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 13), 0.0, 5.0))
                << (SaleHistory(Item(ID("s1"), ID("p03")))
                    << SaleHistoryDay(Item(ID("s1"), ID("p03")), Date(2015, 8, 10), 4.0, 1.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p03")), Date(2015, 8, 11), 0.0, 1.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p03")), Date(2015, 8, 12), 2.0, 5.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p03")), Date(2015, 8, 13), 0.0, 5.0))
                << (SaleHistory(Item(ID("s2"), ID("p1")))
                    << SaleHistoryDay(Item(ID("s2"), ID("p1")), Date(2015, 8, 10), 0.0, 0.0)
                    << SaleHistoryDay(Item(ID("s2"), ID("p1")), Date(2015, 8, 11), 0.0, 0.0)
                    << SaleHistoryDay(Item(ID("s2"), ID("p1")), Date(2015, 8, 12), 2.0, 4.0)
                    << SaleHistoryDay(Item(ID("s2"), ID("p1")), Date(2015, 8, 13), 0.0, 4.0)));

    QTest::newRow("different_analogs_2")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 10), 1.0, 2.0)
                << SaleHistoryDay(Item(ID("s2"), ID("p1")), Date(2015, 8, 12), 2.0, 4.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p2")), Date(2015, 8, 10), 4.0, 0.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p3")), Date(2015, 8, 12), 2.0, 4.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p4")), Date(2015, 8, 10), 4.0, 1.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p2")), Date(2015, 8, 11), 1.0, 3.0))
            << (AnalogsTable()
                << (Analogs("p1")
                    << ID("p2"))
                << (Analogs("p03")
                    << ID("p4")
                    << ID("p3")))

            << (QList<Item>() << Item(ID("s1"), ID("p1"))
                << Item(ID("s2"), ID("p1"))
                << Item(ID("s1"), ID("p2"))
                << Item(ID("s1"), ID("p4")))
            << Date(2015, 8, 10)
            << Date(2015, 8, 13)

            << (QSet<SaleHistory>()
                << (SaleHistory(Item(ID("s1"), ID("p1")))
                    << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 10), 5.0, 2.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 11), 1.0, 5.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 12), 0.0, 5.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 13), 0.0, 5.0))
                << (SaleHistory(Item(ID("s1"), ID("p03")))
                    << SaleHistoryDay(Item(ID("s1"), ID("p03")), Date(2015, 8, 10), 4.0, 1.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p03")), Date(2015, 8, 11), 0.0, 1.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p03")), Date(2015, 8, 12), 2.0, 5.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p03")), Date(2015, 8, 13), 0.0, 5.0))
                << (SaleHistory(Item(ID("s2"), ID("p1")))
                    << SaleHistoryDay(Item(ID("s2"), ID("p1")), Date(2015, 8, 10), 0.0, 0.0)
                    << SaleHistoryDay(Item(ID("s2"), ID("p1")), Date(2015, 8, 11), 0.0, 0.0)
                    << SaleHistoryDay(Item(ID("s2"), ID("p1")), Date(2015, 8, 12), 2.0, 4.0)
                    << SaleHistoryDay(Item(ID("s2"), ID("p1")), Date(2015, 8, 13), 0.0, 4.0)));

}

