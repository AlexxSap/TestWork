#include "BenchmarkWriteRead.h"
/*
результаты даны в мс
дни/склады/товары		30/1/100		90/1/100		30/10/100		30/1/1000
запись/чтение           З	Ч			З	Ч			З	Ч			З	Ч

Первоначальные данные	109	2296		287	8456		981	21728		909	21706
Изменён метод SalesHistoryStreamReader::queryForNextItem()
                        134	823         292	4374        928	8023        943	8206
Добавлен индекс i_datas	146	793         323	4478        1080 8010       1085 7976

*/

void BenchmarkWriteRead::run(const int &days, const int &storages, const int &products)
{
    const Date fromDate = Date(2015, 1, 1);
    const Date toDate = fromDate.addDays(days - 1);
    const QString dbName("TestDBase.db");

    qInfo() << "-------Benchmark for write and read data-------" << endl
            << days << " days, "
            << storages << " storages, "
            << products << " products";

    if(!TestUtility::removeFile(dbName))
    {
        qWarning() << "cannot remove test-db in begining of benchmark";
        return;
    }

    if(!TestUtility::createTestDB(dbName))
    {
        qWarning() << "cannot create test-db";
        return;
    }

    QElapsedTimer timer;
    const SaleHistoryGenerator gen;
    const QList<SaleHistoryDay> list = gen.generateHistory(fromDate,
                                                           toDate,
                                                           storages,
                                                           products);
    bool result = false;
    {
        SaleHistoryWriter writer(dbName);

        timer.start();
        result = writer.write(list);
        qInfo() << "write............." << timer.elapsed() << "ms";
    }
    if(!result)
    {
        qWarning() << "cannot write data to db";
        return;
    }

    QList<Item> items;
    foreach (const SaleHistoryDay &day, list)
    {
        items.append(day.item());
    }

    QList<SaleHistory> saleHistoryList;
    SalesHistoryStreamReader reader(items, dbName);

    timer.start();
    bool isOpen = reader.open(fromDate, toDate);
    const int openTime = timer.elapsed();

    if(!isOpen)
    {
        TestUtility::removeFile(dbName);
        qWarning() << "something wrong with opening SalesHistoryStreamReader";
        return;
    }
    timer.start();
    do
    {
        const SaleHistory history = reader.current();
        saleHistoryList.append(history);
    } while (reader.next());

    qInfo() << "read.............."
            << timer.elapsed() + openTime << "ms";

    if(!TestUtility::removeFile(dbName))
    {
        qWarning() << "cannot remove test-db in ending of benchmark";
        return;
    }

    qInfo() << "----End of benchmark for write and read data---" << endl;
}
