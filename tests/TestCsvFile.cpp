#include "TestCsvFile.h"

TestCsvFile::TestCsvFile(QObject *parent) : QObject(parent)
{

}

void TestCsvFile::testCsvFile()
{
    QFETCH(QList<SaleHistoryDay>, inData);
    QFETCH(QSet<SaleHistoryDay>, expData);

    const QString fileName(QString(QTest::currentDataTag()) + "tf.csv");
    const QString dbName(QString(QTest::currentDataTag()) + "tdb");

    DataBaseInfo info;
    info.setDataBaseName(dbName);
    info.setUserName("root");
    info.setPassword("1234");

    if(!TestUtility::removeFile(fileName))
    {
        QFAIL("cannot remove test-file in begin of test");
    }

    if(!DataBase::remDataBase(info))
    {
        QFAIL("cannot remove test-db in begin of test");
    }

    const bool isWrite = CsvFile::write(inData, fileName);
    if(!isWrite)
    {
        QFAIL("cannot write data to test file");
    }

    const QList<SaleHistoryDay> actData = CsvFile::read(fileName);

    QCOMPARE(actData.toSet(), expData);
    {
        SaleHistoryWriter writer(info);
        bool isWritedToDb = writer.importFromFile(fileName);

        if(!isWritedToDb)
        {
            QFAIL(QString("cannot write to file " + fileName).toLocal8Bit());
        }
    }

    QList<Item> items;
    foreach (const SaleHistoryDay &day, actData)
    {
        const Item item = day.item();
        if(!items.contains(item))
        {
            items.append(item);
        }
    }

    {
        QList<SaleHistoryDay> days;
        SalesHistoryStreamReader reader(items, info);
        const bool isOpen = reader.open(Date(), Date());
        if(isOpen)
        {
            do
            {
                days.append(reader.current().days());
            }
            while(reader.next());
        }

        QCOMPARE(days.toSet(), expData);

    }

    if(!DataBase::remDataBase(info))
    {
        QFAIL("cannot remove test-db in end of test");
    }

    if(!TestUtility::removeFile(fileName))
    {
        QFAIL("cannot remove test-file in end of test");
    }
}

void TestCsvFile::testCsvFile_data()
{
    QTest::addColumn<QList<SaleHistoryDay> >("inData");
    QTest::addColumn<QSet<SaleHistoryDay> >("expData");

    QTest::newRow("empty1") << QList<SaleHistoryDay>()
                            << QSet<SaleHistoryDay>();


    QTest::newRow("simple") << (QList<SaleHistoryDay>()
                                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                                << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 10), 220.0, 11.0)
                                << SaleHistoryDay(Item(ID("storage2"), ID("product2")), Date(2015, 8, 10), 2.0, 1.0))

                            << (QSet<SaleHistoryDay>()
                                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                                << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 10), 220.0, 11.0)
                                << SaleHistoryDay(Item(ID("storage2"), ID("product2")), Date(2015, 8, 10), 2.0, 1.0));

    QTest::newRow("empty") << (QList<SaleHistoryDay>()
                               << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                               << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                               << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                               << SaleHistoryDay()
                               << SaleHistoryDay(Item(ID("storage2"), ID("product2")), Date(2015, 8, 10), 2.0, 1.0))

                           << (QSet<SaleHistoryDay>()
                               << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                               << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                               << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                               << SaleHistoryDay(Item(ID("storage2"), ID("product2")), Date(2015, 8, 10), 2.0, 1.0));

}
