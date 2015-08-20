#include "TestSalesHistoryStreamReader.h"

TestSalesHistoryStreamReader::TestSalesHistoryStreamReader(QObject *parent) : QObject(parent)
{

}

void TestSalesHistoryStreamReader::testSalesHistoryStreamReader()
{    
    QFETCH(QList<SaleHistoryDay> , data);
    QFETCH(QList<Item>, caseItems);
    QFETCH(Date, caseDateFrom);
    QFETCH(Date, caseDateTo);
    QFETCH(QList<SaleHistory> , expList);

    const QString dbName(QString(QTest::currentDataTag()) + "TestDBase.db");

    if(!TestUtility::removeFile(dbName))
    {
        QFAIL("cannot remove test-db in begining of test");
    }

    bool result = false;
    {
        SaleHistoryWriter writer(dbName);
        result = writer.write(data);
    }

    if(!result)
    {
        TestUtility::removeFile(dbName);
        QFAIL("cannot write data to db");
    }
    qInfo() << "data is writed";

    if(caseItems.isEmpty())
    {
        foreach (const SaleHistoryDay &day, data)
        {
            const Item item = day.item();
            if(!caseItems.contains(item))
            {
                caseItems.append(item);
            }
        }
    }

    //qInfo() << caseItems;

    QList<SaleHistory> actList;
    {
        SalesHistoryStreamReader reader(caseItems, dbName);
        bool isOpen = reader.open(caseDateFrom, caseDateTo);
        if(!isOpen)
        {
            TestUtility::removeFile(dbName);
            QFAIL("something wrong with opening SalesHistoryStreamReader");
        }
        qInfo() << "SalesHistoryStreamReader is open";
        do
        {
            const SaleHistory history = reader.current();
            actList.append(history);
        }while (reader.next());
    }

    if(!TestUtility::removeFile(dbName))
    {
        QFAIL("cannot remove test-db in ending of test");
    }

    bool res = actList == expList;

    if(!res)
    {
        qInfo() << "actList - " << actList;
        qInfo() << "expList - " << expList;
    }
    QCOMPARE(actList, expList);
}

void TestSalesHistoryStreamReader::testSalesHistoryStreamReader_data()
{
    QTest::addColumn<QList<SaleHistoryDay> >("data");

    QTest::addColumn<QList<Item> >("caseItems");
    QTest::addColumn<Date>("caseDateFrom");
    QTest::addColumn<Date>("caseDateTo");

    QTest::addColumn<QList<SaleHistory> >("expList");

    QTest::newRow("simple")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 10), 220.0, 11.0)
                << SaleHistoryDay(Item(ID("storage2"), ID("product2")), Date(2015, 8, 10), 2.0, 1.0))

            << (QList<Item>() << Item(ID("storage1"), ID("product1"))
                << Item(ID("storage1"), ID("product2")))
            << Date(2015, 8, 10)
            << Date(2015, 8, 11)

            << (QList<SaleHistory>()
                << (SaleHistory(Item(ID("storage1"), ID("product1")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0))
                << (SaleHistory(Item(ID("storage1"), ID("product2")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 10), 220.0, 11.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 11), 0.0, 11.0)));

    QTest::newRow("simple wo date")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 10), 220.0, 11.0)
                << SaleHistoryDay(Item(ID("storage2"), ID("product2")), Date(2015, 8, 10), 2.0, 1.0))

            << (QList<Item>() << Item(ID("storage1"), ID("product1"))
                << Item(ID("storage1"), ID("product2")))
            << Date()
            << Date()

            << (QList<SaleHistory>()
                << (SaleHistory(Item(ID("storage1"), ID("product1")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0))
                << (SaleHistory(Item(ID("storage1"), ID("product2")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 10), 220.0, 11.0)));

    QTest::newRow("simple wo items")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 10), 220.0, 11.0)
                << SaleHistoryDay(Item(ID("storage2"), ID("product2")), Date(2015, 8, 10), 2.0, 1.0))

            << QList<Item>()
            << Date(2015, 8, 10)
            << Date(2015, 8, 11)

            << (QList<SaleHistory>()
                << (SaleHistory(Item(ID("storage1"), ID("product1")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0))
                << (SaleHistory(Item(ID("storage1"), ID("product2")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 10), 220.0, 11.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 11), 0.0, 11.0))
                << (SaleHistory(Item(ID("storage2"), ID("product2")))
                    << SaleHistoryDay(Item(ID("storage2"), ID("product2")), Date(2015, 8, 10), 2.0, 1.0)
                    << SaleHistoryDay(Item(ID("storage2"), ID("product2")), Date(2015, 8, 11), 0.0, 1.0)));

    QTest::newRow("empty Item")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 10), 220.0, 11.0)
                << SaleHistoryDay(Item(ID("storage2"), ID("product2")), Date(2015, 8, 10), 2.0, 1.0))

            << (QList<Item>() << Item(ID("storage1"), ID("product3"))
                << Item(ID("storage1"), ID("product1")))
            << Date(2015, 8, 10)
            << Date(2015, 8, 12)

            << (QList<SaleHistory>()
                << (SaleHistory(Item(ID("storage1"), ID("product3")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 8, 10), 0.0, 0.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 8, 11), 0.0, 0.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 8, 12), 0.0, 0.0))

                << (SaleHistory(Item(ID("storage1"), ID("product1")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)));

    QTest::newRow("empty case items and date")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 10), 220.0, 11.0)
                << SaleHistoryDay(Item(ID("storage2"), ID("product2")), Date(2015, 8, 10), 2.0, 1.0))

            << QList<Item>()
            << Date()
            << Date()

            << (QList<SaleHistory>()
                << (SaleHistory(Item(ID("storage1"), ID("product1")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0))

                << (SaleHistory(Item(ID("storage1"), ID("product2")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 10), 220.0, 11.0))

                << (SaleHistory(Item(ID("storage2"), ID("product2")))
                    << SaleHistoryDay(Item(ID("storage2"), ID("product2")), Date(2015, 8, 10), 2.0, 1.0)));

    QTest::newRow("empty case items and first date")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 8), 20.0, 5.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 10), 220.0, 11.0)
                << SaleHistoryDay(Item(ID("storage2"), ID("product2")), Date(2015, 8, 11), 2.0, 1.0)
                << SaleHistoryDay(Item(ID("storage2"), ID("product2")), Date(2015, 8, 12), 212.0, 13.0))

            << QList<Item>()
            << Date()
            << Date(2015, 8, 11)

            << (QList<SaleHistory>()
                << (SaleHistory(Item(ID("storage1"), ID("product1")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0))
                << (SaleHistory(Item(ID("storage1"), ID("product2")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 8), 20.0, 5.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 9), 0.0, 5.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 10), 220.0, 11.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 11), 0.0, 11.0))
                << (SaleHistory(Item(ID("storage2"), ID("product2")))
                    << SaleHistoryDay(Item(ID("storage2"), ID("product2")), Date(2015, 8, 11), 2.0, 1.0)));

    QTest::newRow("rests for skipped days")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0))

            << QList<Item>()
            << Date(2015, 8, 9)
            << Date(2015, 8, 11)

            << (QList<SaleHistory>()
                << (SaleHistory(Item(ID("storage1"), ID("product1")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 9), 0.0, 0.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 0.0, 20.0)));

    QTest::newRow("Item not found")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 10), 220.0, 11.0)
                << SaleHistoryDay(Item(ID("storage2"), ID("product3")), Date(2015, 8, 10), 2.0, 1.0))

            << (QList<Item>() << Item(ID("storage2"), ID("product2")))
            << Date()
            << Date()

            << (QList<SaleHistory>()
                << SaleHistory(Item(ID("storage2"), ID("product2"))));

    QTest::newRow("diff Items")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage2"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 11), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage2"), ID("product2")), Date(2015, 8, 10), 220.0, 11.0))

            << (QList<Item>() << Item(ID("storage1"), ID("product2"))
                << Item(ID("storage2"), ID("product1")))
            << Date()
            << Date()

            << (QList<SaleHistory>()
                << (SaleHistory(Item(ID("storage1"), ID("product2")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 11), 20.0, 10.0))
                << (SaleHistory(Item(ID("storage2"), ID("product1")))
                    << SaleHistoryDay(Item(ID("storage2"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)));

    QTest::newRow("first date not ini")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 13), 220.0, 11.0)
                << SaleHistoryDay(Item(ID("storage2"), ID("product3")), Date(2015, 8, 10), 2.0, 1.0))

            << (QList<Item>() << Item(ID("storage1"), ID("product1")))
            << Date()
            << Date(2015, 8, 12)

            << (QList<SaleHistory>()
                << (SaleHistory(Item(ID("storage1"), ID("product1")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)));

    QTest::newRow("second date not ini")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 13), 220.0, 11.0)
                << SaleHistoryDay(Item(ID("storage2"), ID("product3")), Date(2015, 8, 10), 2.0, 1.0))

            << (QList<Item>() << Item(ID("storage1"), ID("product1")))
            << Date(2015, 8, 11)
            << Date()

            << (QList<SaleHistory>()
                << (SaleHistory(Item(ID("storage1"), ID("product1")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 13), 220.0, 11.0)));

    QTest::newRow("not full data")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 8, 10), 50.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 8, 13), 10.0, 0.0))

            << (QList<Item>() << Item(ID("storage1"), ID("product3")))
            << Date(2015, 8, 10)
            << Date(2015, 8, 13)

            << (QList<SaleHistory>()
                << (SaleHistory(Item(ID("storage1"), ID("product3")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 8, 10), 50.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 8, 11), 0.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 8, 12), 0.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 8, 13), 10.0, 0.0)));


}
