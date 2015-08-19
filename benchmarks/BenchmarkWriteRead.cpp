#include "BenchmarkWriteRead.h"
/*
результаты даны в мс
дни/склады/товары		30/1/100		90/1/100		30/10/100		30/1/1000   720/10/10000
запись/чтение           З	Ч			З	Ч			З	Ч			З	Ч       з   ч

Первоначальные данные	109	2296		287	8456		981	21728		909	21706   43 min	18 h
Изменён метод SalesHistoryStreamReader::queryForNextItem()
                        134	823         292	4374        928	8023        943	8206    43 min	5-6 h
Добавлен индекс i_datas	146	793         323	4478        1080 8010       1085 7976
(выигрыш в чтении находится в рамках отклонения, не использовать)
Изменение селект-запроса в методе SalesHistoryStreamReader::open
                        116	15          293	42          912	140         931	143     43 min	14 min
Исравление ошибки бенчмарка
                        126	15          330	42          1055 140        1038 143
Изменение записи в БД на пакетную
                        37	15          113	42          345	140         347	143     15 min	7 min
Исправление бенчмарка и селекта в классе SalesHistoryStreamReader
                        250 33          722 77          2486 216        2964 216    57 min 5 min

*/

void BenchmarkWriteRead::run(const int &days, const int &storages, const int &products)
{
    const Date fromDate = Date(2015, 1, 1);
    const Date toDate = fromDate.addDays(days - 1);
    const QString dbName("TestDBase.db");
    const QString fileName("testFile.csv");

    qint64 writeTime = 0;
    qint64 readTime = 0;

    qInfo() << "-------Benchmark for write and read data-------" << endl
            << days << " days, "
            << storages << " storages, "
            << products << " products";

    if(!TestUtility::removeFile(dbName))
    {
        qWarning() << "cannot remove test-db in begining of benchmark";
        return;
    }

    if(!TestUtility::removeFile(fileName))
    {
        qWarning() << "cannot remove test-file in begining of benchmark";
        return;
    }

    QElapsedTimer timer;
    QList<Item> items;
    bool result = false;
    {
        const SaleHistoryGenerator gen;
        const int monthCount = 1;

        for(Date date = fromDate; date < toDate; date = date.addMonths(monthCount).addDays(1))
        {
            const QList<SaleHistoryDay> list = gen.generateHistory(date,
                                                               date.addMonths(monthCount),
                                                               storages,
                                                               products);

            qInfo() << "write data to file" << date <<  date.addMonths(monthCount);

            foreach (const SaleHistoryDay &day, list)
            {
                const Item item = day.item();
                if(!items.contains(item))
                {
                    items.append(item);
                }
            }

            bool isWrited = CsvFile::write(list, fileName);
            if(!isWrited)
            {
                qWarning() << "cannot write to file";
                return;
            }
            qInfo() << "writed";
        }

        SaleHistoryWriter writer(dbName);
        timer.start();
        qInfo() << "import from file";
        result = writer.importFromFile(fileName);
        writeTime = timer.elapsed();
        qInfo() << "write............." << writeTime << "ms";
    }
    if(!result)
    {
        qWarning() << "cannot write data to db";
        return;
    }

    {
        SalesHistoryStreamReader reader(items, dbName);

        timer.start();
        qInfo() << "open reader";
        bool isOpen = reader.open(Date(), Date());
        const int openTime = timer.elapsed();

        if(!isOpen)
        {
            TestUtility::removeFile(dbName);
            TestUtility::removeFile(fileName);
            qWarning() << "something wrong with opening SalesHistoryStreamReader";
            return;
        }
        timer.start();
        qInfo() << "begin read";
        int counter = 0;
        do
        {
            const SaleHistory history = reader.current();
            counter++;
            if(counter%1000 == 0)
            {
                qInfo() << "SaleHistory - " << counter;
            }
        } while (reader.next());

        readTime = timer.elapsed() + openTime;
        qInfo() << "read.............."
                << readTime << "ms";
    }

    if(!TestUtility::removeFile(dbName))
    {
        qWarning() << "cannot remove test-db in ending of benchmark";
        return;
    }

    if(!TestUtility::removeFile(fileName))
    {
        qWarning() << "cannot remove test-file in ending of benchmark";
        return;
    }

    qInfo() << "for 10000 products-------";
    writeTime = 10000/products * writeTime * 1.8;
    readTime = 10000/products * readTime;
    qInfo() << "write " << writeTime << "ms or " << writeTime / 60000 << "min";
    qInfo() << "read " << readTime << "ms or " << readTime / 60000 << "min";

    qInfo() << "----End of benchmark for write and read data---" << endl;
}
