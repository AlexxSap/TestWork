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
    QFETCH(QSet<SaleHistory> , expList);

    const QString dbName(QString(QTest::currentDataTag()) + "tdb");

    DataBaseInfo info;
    info.setDataBaseName(dbName);
    info.setUserName("root");
    info.setPassword("1234");

    if(!DataBase::remDataBase(info))
    {
        QFAIL("cannot remove test-db in begining of test");
    }

    bool result = false;
    {
        SaleHistoryWriter writer(info);
        result = writer.write(data);
    }

    if(!result)
    {
        DataBase::remDataBase(info);
        QFAIL("cannot write data to db");
    }

    QList<SaleHistory> actList;
    {
        SalesHistoryStreamReader reader(caseItems, info);
        bool isOpen = reader.open(caseDateFrom, caseDateTo);
        if(isOpen)
        {
            do
            {
                const SaleHistory history = reader.current();
                actList.append(history);
            }while (reader.next());
        }
    }

    if(!DataBase::remDataBase(info))
    {
        QFAIL("cannot remove test-db in ending of test");
    }
    QCOMPARE(actList.toSet(), expList);
}

void TestSalesHistoryStreamReader::testSalesHistoryStreamReader_data()
{
    QTest::addColumn<QList<SaleHistoryDay> >("data");

    QTest::addColumn<QList<Item> >("caseItems");
    QTest::addColumn<Date>("caseDateFrom");
    QTest::addColumn<Date>("caseDateTo");

    QTest::addColumn<QSet<SaleHistory> >("expList");

    QTest::newRow("empty_data")
            << QList<SaleHistoryDay>()

            << (QList<Item>() << Item(ID("storage1"), ID("продукт1"))
                << Item(ID("storage1"), ID("product2")))
            << Date(2015, 8, 10)
            << Date(2015, 8, 11)

            << (QSet<SaleHistory>()
                << (SaleHistory(Item(ID("storage1"), ID("продукт1")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("продукт1")), Date(2015, 8, 10), 0.0, 0.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("продукт1")), Date(2015, 8, 11), 0.0, 0.0))
                << (SaleHistory(Item(ID("storage1"), ID("product2")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 10), 0.0, 0.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 11), 0.0, 0.0)));

    QTest::newRow("simple")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 10), 220.0, 11.0)
                << SaleHistoryDay(Item(ID("storage2"), ID("Продукт2©")), Date(2015, 8, 11), 2.0, 1.0))

            << (QList<Item>() << Item(ID("storage1"), ID("product1"))
                << Item(ID("storage1"), ID("Продукт2©")))
            << Date(2015, 8, 10)
            << Date(2015, 8, 11)

            << (QSet<SaleHistory>()
                << (SaleHistory(Item(ID("storage1"), ID("product1")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0))
                << (SaleHistory(Item(ID("storage1"), ID("Продукт2©")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 10), 220.0, 11.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 11), 0.0, 11.0)));

    QTest::newRow("simple_large")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 10), 220.0, 11.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 11), 2.0, 1.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 13), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 14), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 15), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 16), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 17), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 18), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 19), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 20), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 21), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 22), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 23), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 25), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 26), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 27), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 28), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 20), 20.0, 10.0) )

            << (QList<Item>() << Item(ID("storage1"), ID("product1"))
                << Item(ID("storage1"), ID("Продукт2©")))
            << Date(2015, 8, 10)
            << Date(2015, 8, 28)

            << (QSet<SaleHistory>()
                << (SaleHistory(Item(ID("storage1"), ID("product1")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 13), 50.0, 20.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 14), 20.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 15), 10.0, 0.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 16), 50.0, 20.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 17), 20.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 18), 10.0, 0.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 19), 50.0, 20.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 20), 20.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 21), 10.0, 0.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 22), 50.0, 20.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 23), 20.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 24), 0.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 25), 10.0, 0.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 26), 50.0, 20.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 27), 20.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 28), 10.0, 0.0))
                << (SaleHistory(Item(ID("storage1"), ID("Продукт2©")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 10), 220.0, 11.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 11), 2.0, 1.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 12), 0.0, 1.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 13), 0.0, 1.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 14), 0.0, 1.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 15), 0.0, 1.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 16), 0.0, 1.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 17), 0.0, 1.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 18), 0.0, 1.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 19), 0.0, 1.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 20), 20.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 21), 0.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 22), 0.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 23), 0.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 24), 0.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 25), 0.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 26), 0.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 27), 0.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("Продукт2©")), Date(2015, 8, 28), 0.0, 10.0)));

    QTest::newRow("simple_wo_date")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage1"), ID("productՊ")), Date(2015, 8, 10), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("productՊ")), Date(2015, 8, 11), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("productՊ")), Date(2015, 8, 12), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 10), 220.0, 11.0)
                << SaleHistoryDay(Item(ID("storage2"), ID("product2")), Date(2015, 8, 10), 2.0, 1.0))

            << (QList<Item>() << Item(ID("storage1"), ID("productՊ"))
                << Item(ID("storage1"), ID("product2")))
            << Date()
            << Date()

            << (QSet<SaleHistory>()
                << (SaleHistory(Item(ID("storage1"), ID("productՊ")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("productՊ")), Date(2015, 8, 10), 50.0, 20.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("productՊ")), Date(2015, 8, 11), 20.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("productՊ")), Date(2015, 8, 12), 10.0, 0.0))
                << (SaleHistory(Item(ID("storage1"), ID("product2")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 10), 220.0, 11.0)));

    QTest::newRow("simple_wo_items")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 10), 220.0, 11.0)
                << SaleHistoryDay(Item(ID("storage2"), ID("product2")), Date(2015, 8, 10), 2.0, 1.0))

            << QList<Item>()
            << Date(2015, 8, 10)
            << Date(2015, 8, 11)

            << QSet<SaleHistory>();

    QTest::newRow("empty_Item")
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

            << (QSet<SaleHistory>()
                << (SaleHistory(Item(ID("storage1"), ID("product3")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 8, 10), 0.0, 0.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 8, 11), 0.0, 0.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 8, 12), 0.0, 0.0))

                << (SaleHistory(Item(ID("storage1"), ID("product1")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)));

    QTest::newRow("empty_first_date")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 8), 20.0, 5.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 10), 220.0, 11.0)
                << SaleHistoryDay(Item(ID("storage2"), ID("product2")), Date(2015, 8, 11), 2.0, 1.0)
                << SaleHistoryDay(Item(ID("storage2"), ID("product2")), Date(2015, 8, 12), 212.0, 13.0))

            << (QList<Item>()
                << Item(ID("storage1"), ID("product1"))
                << Item(ID("storage1"), ID("product2"))
                << Item(ID("storage2"), ID("product2")))
            << Date()
            << Date(2015, 8, 11)

            << (QSet<SaleHistory>()
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

    QTest::newRow("rests_for_skip_day")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0))

            << (QList<Item>()
                << Item(ID("storage1"), ID("product1")))
            << Date(2015, 8, 9)
            << Date(2015, 8, 11)

            << (QSet<SaleHistory>()
                << (SaleHistory(Item(ID("storage1"), ID("product1")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 9), 0.0, 0.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 0.0, 20.0)));

    QTest::newRow("Item_not_found")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 10), 220.0, 11.0)
                << SaleHistoryDay(Item(ID("storage2"), ID("product3")), Date(2015, 8, 10), 2.0, 1.0))

            << (QList<Item>() << Item(ID("storage2"), ID("product2")))
            << Date()
            << Date()

            << (QSet<SaleHistory>()
                << SaleHistory(Item(ID("storage2"), ID("product2"))));

    QTest::newRow("diff_Items")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage2"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 11), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage2"), ID("product2")), Date(2015, 8, 10), 220.0, 11.0))

            << (QList<Item>() << Item(ID("storage1"), ID("product2"))
                << Item(ID("storage2"), ID("product1")))
            << Date()
            << Date()

            << (QSet<SaleHistory>()
                << (SaleHistory(Item(ID("storage1"), ID("product2")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 11), 20.0, 10.0))
                << (SaleHistory(Item(ID("storage2"), ID("product1")))
                    << SaleHistoryDay(Item(ID("storage2"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)));

    QTest::newRow("first_date_not_ini")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 13), 220.0, 11.0)
                << SaleHistoryDay(Item(ID("storage2"), ID("product3")), Date(2015, 8, 10), 2.0, 1.0))

            << (QList<Item>() << Item(ID("storage1"), ID("product1")))
            << Date()
            << Date(2015, 8, 12)

            << (QSet<SaleHistory>()
                << (SaleHistory(Item(ID("storage1"), ID("product1")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)));

    QTest::newRow("second_date_not_ini")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 13), 220.0, 11.0)
                << SaleHistoryDay(Item(ID("storage2"), ID("product3")), Date(2015, 8, 10), 2.0, 1.0))

            << (QList<Item>() << Item(ID("storage1"), ID("product1")))
            << Date(2015, 8, 11)
            << Date()

            << (QSet<SaleHistory>()
                << (SaleHistory(Item(ID("storage1"), ID("product1")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 13), 220.0, 11.0)));

    QTest::newRow("not_full_data")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 8, 10), 50.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 8, 13), 10.0, 0.0))

            << (QList<Item>() << Item(ID("storage1"), ID("product3")))
            << Date(2015, 8, 10)
            << Date(2015, 8, 13)

            << (QSet<SaleHistory>()
                << (SaleHistory(Item(ID("storage1"), ID("product3")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 8, 10), 50.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 8, 11), 0.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 8, 12), 0.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 8, 13), 10.0, 0.0)));

    QTest::newRow("invalid_date")
            << (QList<SaleHistoryDay>()
                << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 2, 27), 51.0, 1.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 2, 28), 50.0, 10.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 3, 1), 10.0, 0.0)
                << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 3, 2), 11.0, 3.0))

            << (QList<Item>() << Item(ID("storage1"), ID("product3")))
            << Date(2015, 2, 28)
            << Date(2015, 2, 29)

            << (QSet<SaleHistory>()
                << (SaleHistory(Item(ID("storage1"), ID("product3")))
                    << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 2, 28), 50.0, 10.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 3, 1), 10.0, 0.0)
                    << SaleHistoryDay(Item(ID("storage1"), ID("product3")), Date(2015, 3, 2), 11.0, 3.0)));

}
