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
    QFETCH(QList<SaleHistory>, expResult);

    const QString dbName(QString(QTest::currentDataTag()) + "TestDBase.db");

    if(!TestUtility::removeFile(dbName))
    {
        QFAIL("cannot remove test-db in begin of test");
    }

    bool result = false;
    {
        SaleHistoryWriter writer(dbName);
        result = writer.write(history);
    }
    if(!result)
    {
        TestUtility::removeFile(dbName);
        QFAIL("cannot write history to db");
    }

    {
        AnalogsWriter writer(dbName);
        result = writer.write(analogsTable);
    }
    if(!result)
    {
        TestUtility::removeFile(dbName);
        QFAIL("cannot write analogs to db");
    }

    QList<SaleHistory> actResult;
    {
        SalesHistoryStreamReader reader(caseItems, dbName);
        bool isOpen = reader.open(caseDateFrom, caseDateTo, true);
        if(!isOpen)
        {
            TestUtility::removeFile(dbName);
            QFAIL("something wrong with opening SalesHistoryStreamReader");
        }
        do
        {
            const SaleHistory curHistory = reader.current();
            //            qInfo() << curHistory;
            actResult.append(curHistory);
        }while (reader.next());
    }

    if(!TestUtility::removeFile(dbName))
    {
        QFAIL("cannot remove test-db in end of test");
    }

    bool res = actResult == expResult;

    if(!res)
    {
        qInfo() << "actResult - " << actResult;
        qInfo() << "expResult - " << expResult;
    }
    QCOMPARE(actResult, expResult);
}

void TSalesHistoryReaderWithAnalogs::TestSalesHistoryReaderWithAnalogs_data()
{
    QTest::addColumn< QList<SaleHistoryDay> >("history");
    QTest::addColumn<AnalogsTable>("analogsTable");

    QTest::addColumn< QList<Item> >("caseItems");
    QTest::addColumn<Date>("caseDateFrom");
    QTest::addColumn<Date>("caseDateTo");

    QTest::addColumn< QList<SaleHistory> >("expResult");

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

            << (QList<SaleHistory>()
                << (SaleHistory(Item(ID("s1"), ID("p01")))
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 10), 5.0, 2.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 11), 1.0, 5.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 12), 2.0, 7.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p01")), Date(2015, 8, 13), 0.0, 7.0)));

    QTest::newRow("different storage")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 10), 1.0, 2.0)
                << SaleHistoryDay(Item(ID("s2"), ID("p1")), Date(2015, 8, 12), 2.0, 4.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p2")), Date(2015, 8, 10), 4.0, 0.0)
                << SaleHistoryDay(Item(ID("s1"), ID("p2")), Date(2015, 8, 11), 1.0, 3.0))
            << (AnalogsTable() << (Analogs("p1")
                                   << ID("p2")))

            << (QList<Item>() << Item(ID("s1"), ID("p1"))
                << Item(ID("s2"), ID("p1"))
                << Item(ID("s1"), ID("p2")))
            << Date(2015, 8, 10)
            << Date(2015, 8, 13)

            << (QList<SaleHistory>()
                << (SaleHistory(Item(ID("s1"), ID("p1")))
                    << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 10), 5.0, 2.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 11), 1.0, 5.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 12), 0.0, 5.0)
                    << SaleHistoryDay(Item(ID("s1"), ID("p1")), Date(2015, 8, 13), 0.0, 5.0))
                << (SaleHistory(Item(ID("s2"), ID("p1")))
                    << SaleHistoryDay(Item(ID("s2"), ID("p1")), Date(2015, 8, 10), 0.0, 0.0)
                    << SaleHistoryDay(Item(ID("s2"), ID("p1")), Date(2015, 8, 11), 0.0, 0.0)
                    << SaleHistoryDay(Item(ID("s2"), ID("p1")), Date(2015, 8, 12), 2.0, 4.0)
                    << SaleHistoryDay(Item(ID("s2"), ID("p1")), Date(2015, 8, 13), 0.0, 4.0)));

    QTest::newRow("different analogs")
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

            << (QList<SaleHistory>()
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

