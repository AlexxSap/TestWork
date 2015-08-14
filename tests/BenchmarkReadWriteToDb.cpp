#include "BenchmarkReadWriteToDb.h"

BenchmarkReadWriteToDb::BenchmarkReadWriteToDb(QObject *parent) : QObject(parent)
{

}

void BenchmarkReadWriteToDb::prepare()
{
    QTest::addColumn<QDate>("fromDate");
    QTest::addColumn<QDate>("toDate");
    QTest::addColumn<int>("storageNum");
    QTest::addColumn<int>("productNum");

    QTest::newRow("10 day / 5 storage / 10 products") << QDate(2015, 3, 1)
                                                      << QDate(2015, 3, 10)
                                                      << 5
                                                      << 10;

    QTest::newRow("20 day / 5 storage / 10 products") << QDate(2015, 3, 1)
                                                      << QDate(2015, 3, 20)
                                                      << 5
                                                      << 10;

    QTest::newRow("20 day / 5 storage / 20 products") << QDate(2015, 3, 1)
                                                      << QDate(2015, 3, 20)
                                                      << 5
                                                      << 20;

    QTest::newRow("20 day / 10 storage / 20 products") << QDate(2015, 3, 1)
                                                       << QDate(2015, 3, 20)
                                                       << 10
                                                       << 20;
}

void BenchmarkReadWriteToDb::testWrite()
{
    QFETCH(QDate, fromDate);
    QFETCH(QDate, toDate);
    QFETCH(int, storageNum);
    QFETCH(int, productNum);

    const int maxVal = 10000;
    const QString dbName("TestDBase.db");

    if(!TestUtility::removeFile(dbName))
    {
        QFAIL("cannot remove test-db in begining of test");
    }

    SaleHistoryGenerator gen;
    gen.setMaxValue(maxVal);
    const QList<SaleHistoryDay> list = gen.generateHistory(fromDate,
                                                           toDate,
                                                           storageNum,
                                                           productNum);
    bool result = false;
    QBENCHMARK
    {
        if(!TestUtility::createTestDB(dbName))
        {
            QFAIL("cannot create test-db");
        }

        {
            SaleHistoryWriter writer(dbName);
            result = writer.write(list);
        }

        if(!result)
        {
            QFAIL("cannot write data to db");
        }

        if(!TestUtility::removeFile(dbName))
        {
            QFAIL("cannot remove test-db");
        }
    }
}

void BenchmarkReadWriteToDb::testWrite_data()
{
    prepare();
}

void BenchmarkReadWriteToDb::testRead()
{
    QFETCH(QDate, fromDate);
    QFETCH(QDate, toDate);
    QFETCH(int, storageNum);
    QFETCH(int, productNum);

    const int maxVal = 10000;
    const QString dbName("TestDBase.db");

    if(!TestUtility::removeFile(dbName))
    {
        QFAIL("cannot remove test-db in begining of test");
    }

    if(!TestUtility::createTestDB(dbName))
    {
        QFAIL("cannot create test-db");
    }

    SaleHistoryGenerator gen;
    gen.setMaxValue(maxVal);
    const QList<SaleHistoryDay> list = gen.generateHistory(fromDate,
                                                           toDate,
                                                           storageNum,
                                                           productNum);
    bool result = false;
    {
        SaleHistoryWriter writer(dbName);
        result = writer.write(list);
    }
    if(!result)
    {
        QFAIL("cannot write data to db");
    }

    QList<Item> items;
    foreach (const SaleHistoryDay &day, list)
    {
        items.append(day.item());
    }

    QBENCHMARK
    {
        QList<SaleHistory> saleHistoryList;
        {
            SalesHistoryStreamReader reader(items);
            reader.setDbName(dbName);
            bool isOpen = reader.open(fromDate, toDate);
            if(!isOpen)
            {
                TestUtility::removeFile(dbName);
                QFAIL("something wrong with opening SalesHistoryStreamReader");
            }
            do
            {
                const SaleHistory history = reader.current();
                saleHistoryList.append(history);
            } while (reader.next());
        }
    }
    if(!TestUtility::removeFile(dbName))
    {
        QFAIL("cannot remove test-db");
    }
}

void BenchmarkReadWriteToDb::testRead_data()
{
    prepare();
}

