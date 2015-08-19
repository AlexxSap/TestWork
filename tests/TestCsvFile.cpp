#include "TestCsvFile.h"

TestCsvFile::TestCsvFile(QObject *parent) : QObject(parent)
{

}

void TestCsvFile::testCsvFile()
{
    QFETCH(QList<SaleHistoryDay>, inData);
    QFETCH(QList<SaleHistoryDay>, expData);

    const QString fileName(QString(QTest::currentDataTag()) + "Test.csv");
    const QString dbName(QString(QTest::currentDataTag()) + "TestDB.db");

    if(!TestUtility::removeFile(fileName))
    {
        QFAIL("cannot remove test-file in begining of test");
    }

    if(!TestUtility::removeFile(dbName))
    {
        QFAIL("cannot remove test-db in begining of test");
    }

    const bool isWrite = CsvFile::write(inData, fileName);
    if(!isWrite)
    {
        QFAIL("cannot write data to test file");
    }

    const QList<SaleHistoryDay> actData = CsvFile::read(fileName);

    QCOMPARE(actData, expData);

    {
        SaleHistoryWriter writer(dbName);
        bool isWritedToDb = writer.importFromFile(fileName);

        if(!isWritedToDb)
        {
            QFAIL(QString("cannot write to file " + fileName).toLocal8Bit());
        }
    }
    {
        SalesHistoryStreamReader reader(QList<Item>(), dbName);
        const bool isOpen = reader.open(Date(), Date());
        if(!isOpen)
        {
            QFAIL(QString("cannot open db " + dbName).toLocal8Bit());
        }

        QList<SaleHistoryDay> days;
        do
        {
            days.append(reader.current().days());
        }
        while(reader.next());

        bool isEqual = TestUtility::compareListWithoutOrder(days, expData);
        QVERIFY(isEqual);
    }

    if(!TestUtility::removeFile(dbName))
    {
        QFAIL("cannot remove test-db in begining of test");
    }

    if(!TestUtility::removeFile(fileName))
    {
        QFAIL("cannot remove test-file in ending of test");
    }
}

void TestCsvFile::testCsvFile_data()
{
    QTest::addColumn<QList<SaleHistoryDay> >("inData");
    QTest::addColumn<QList<SaleHistoryDay> >("expData");

    QTest::newRow("simple") << (QList<SaleHistoryDay>()
                                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                                << SaleHistoryDay(Item(ID("storage1"), ID("product2")), Date(2015, 8, 10), 220.0, 11.0)
                                << SaleHistoryDay(Item(ID("storage2"), ID("product2")), Date(2015, 8, 10), 2.0, 1.0))

                            << (QList<SaleHistoryDay>()
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

                           << (QList<SaleHistoryDay>()
                                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 10), 50.0, 20.0)
                                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 11), 20.0, 10.0)
                                << SaleHistoryDay(Item(ID("storage1"), ID("product1")), Date(2015, 8, 12), 10.0, 0.0)
                                << SaleHistoryDay(Item(ID("storage2"), ID("product2")), Date(2015, 8, 10), 2.0, 1.0));
}
