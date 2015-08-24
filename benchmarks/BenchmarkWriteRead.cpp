#include "BenchmarkWriteRead.h"
/*
------------------данные по замеру быстродействия ---------------
результаты даны в мс                                                                чтение для 5к
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
                        250 33          722 77          2486 216        2964 216    150 min 5 min

Исправление записи в файл и исправление перноса остатков
                        227 10          454 44          1530 61         1547 125    120 min 1 min
Изменение метода SalesHistoryStreamReader::current()
                        245 11          395 23          1427 58         1578 126    50 min  1 min


---------------данные по замеру использования памяти 720 дней 10 складов
читаем с половины складов (если не 1) половину всех товаров за всё время
максимальные значения
10 товаров                  100 товаров                     1000 товаров
Исправление записи в файл и исправление перноса остатков
17MB (5MB) - 20MB (3MB)    61MB (43MB) - 53MB (35MB)      64MB (48MB) - 399MB (372MB)
Изменение метода SalesHistoryStreamReader::current()
17MB (5MB) - 17MB (0MB)    61MB (47MB) - 18MB (0MB)       63MB (49MB) - 27MB (0MB)
1882         73            30818       876                312055      9309
Увеличение размера буффера записи в 3 раза (300000)
18MB (6MB) - 17MB (0MB)    152MB (138MB) - 19MB (0MB)     159MB (144MB) - 29MB (0MB)
1807         73            20638        845                 309908      9338
Для буффера в 1000000
18MB (6MB) - 17MB (0MB)    354MB (340MB) - 26MB (7MB)     479MB (464MB) - 29MB (0MB)
1807         73            15436        863                 239128      9512
разделение процедуры импорта из файла на 2 прохода, замены преобразования даты из строки
16MB (4MB) - 16MB (0MB)    254MB (241MB) - 25MB (6MB)     345MB (330MB) - 104MB (76MB)
1415        74             12215           863             232805         9642
на проверку каждой строки уходит примерно 1/4 часть времени

для 720/10/10000
25 мин - 1 мин

опытные данные для 720/10/10000
356MB (336MB) - 921MB (883MB) - с накоплением
2912563         1822133
2912s           122s

*/

QList<Item> BenchmarkWriteRead::genRandomItemList(const int storages, const int products)
{
    QString productPrefix_("prod_");
    QString storagePrefix_("stor_");

    const int maxStogareCount = storages>1?storages/2:storages;
    const int maxProductCount = products>1?products/2:products;

    QList<Item> list;
    for(int storIndex = 0; storIndex < maxStogareCount; storIndex++)
    {
        for(int prodIndex = 0; prodIndex < maxProductCount; prodIndex++)
        {
            const int storNum = rand() % storages;
            const int prodNum = rand() % products;
            const Item item(storagePrefix_ + QString::number(storNum),
                            productPrefix_ + QString::number(prodNum));

            if(list.contains(item))
            {
                prodIndex--;
            }
            else
            {
                list.append(item);

            }
        }
    }
    return list;
}

void BenchmarkWriteRead::run(const int &days, const int &storages, const int &products)
{
    const Date fromDate = Date(2015, 1, 1);
    const Date toDate = fromDate.addDays(days - 1);
    const QString prefix(QString::number(days) + "_"
                         + QString::number(storages)+ "_"
                         + QString::number(products)+ "_");

    const QString dbName(prefix + "TestDBase.db");
    const QString fileName(prefix + "testFile.csv");

    qint64 writeTime = 0;
    qint64 readTime = 0;

    QThread::msleep(100);
    qInfo() << "-------Benchmark for write and read data-------";
    qInfo() << days << " days, "
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
    QList<Item> items = genRandomItemList(storages, products);

    bool result = false;
    {
        const SaleHistoryGenerator gen;
        const int monthCount = 2;


        for(Date date = fromDate; date < toDate; date = date.addMonths(monthCount).addDays(1))
        {
            const QList<SaleHistoryDay> list = gen.generateHistory(date,
                                                                   date.addMonths(monthCount),
                                                                   storages,
                                                                   products);

            bool isWrited = CsvFile::write(list, fileName);
            if(!isWrited)
            {
                TestUtility::removeFile(dbName);
                TestUtility::removeFile(fileName);
                qWarning() << "cannot write to file";
                return;
            }
        }


        SaleHistoryWriter writer(dbName);
        timer.start();
        const double sWrite = Utils::_runBenchmarking("write");
        result = writer.importFromFile(fileName);
        Utils::_endBenchmarking("write", sWrite);
        writeTime = timer.elapsed();

        qInfo() << "write............." << writeTime << "ms";
    }
    if(!result)
    {
        TestUtility::removeFile(dbName);
        TestUtility::removeFile(fileName);
        qWarning() << "cannot write data to db";
        return;
    }

    {
        SalesHistoryStreamReader reader(items, dbName);

        timer.start();
        bool isOpen = reader.open(Date(), Date());
        const int openTime = timer.elapsed();

        if(!isOpen)
        {
            TestUtility::removeFile(dbName);
            TestUtility::removeFile(fileName);
            qWarning() << "something wrong with opening SalesHistoryStreamReader";
            return;
        }

        const double sRead = Utils::_runBenchmarking("read");
        timer.start();

//        QList<SaleHistory> shList;
        do
        {
            const SaleHistory history = reader.current();
//            shList.append(history);
        } while (reader.next());


        readTime = timer.elapsed() + openTime;
        Utils::_endBenchmarking("read", sRead);
        qInfo() << "read.............."
                << readTime << "ms";

//        QList<Item> actList;
//        foreach (const SaleHistory &history, shList)
//        {
//            if(!actList.contains(history.item()))
//            {
//                actList.append(history.item());
//            }
//        }

//        if(!TestUtility::compareListWithoutOrder(actList, items))
//        {
//            qWarning() << "item lists not equal";
//            qWarning() << "writed list-------";
//            qWarning() << items;
//            qWarning() << "readed list-------";
//            qWarning() << actList;

//        }
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
    writeTime = 10000/products * writeTime;
    readTime = 10000/products * readTime;
    qInfo() << "write " << writeTime << "ms or " << writeTime / 60000 << "min";
    qInfo() << "read " << readTime << "ms or " << readTime / 60000 << "min";

    qInfo() << "----End of benchmark for write and read data---" << endl;
}

/*
Влияние размера буффера записи на время для случая 720/10/100
                100000      10000       1000
первоначальное  17416       43059       68041

Влияние размера буффера записи на память для случая 720/10/100
максимальные значения
                100000          10000           1000
первоначальное  354MB (339MB)   65MB (47MB)     23MB (5MB)
*/

void BenchmarkWriteRead::runForBuffer(const int bufferSize)
{
    const int days = 720;
    const int storages = 10;
    const int products = 100;

    const Date fromDate = Date(2015, 1, 1);
    const Date toDate = fromDate.addDays(days - 1);
    const QString dbName("TestDBase_buffer.db");
    const QString fileName("testFile_buffer.csv");

    qInfo() << "-------Benchmark for import from file -------";
    qInfo() << days << " days, "
            << storages << " storages, "
            << products << " products, "
            << "buffer = " << bufferSize;

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
        const int monthCount = 2;

        for(Date date = fromDate; date < toDate; date = date.addMonths(monthCount).addDays(1))
        {
            const QList<SaleHistoryDay> list = gen.generateHistory(date,
                                                                   date.addMonths(monthCount),
                                                                   storages,
                                                                   products);

            //            qInfo() << "write data to file" << date <<  date.addMonths(monthCount);

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
            //            qInfo() << "writed";
        }
        for(int i = items.count() - 1 ; i >= 0; i -= 2)
        {
            items.removeAt(i);
        }
        //        qInfo() << "will select " <<  items.count() << "items";

        SaleHistoryWriter writer(dbName);
        writer.setBufferSize(bufferSize);
        const double sWrite = Utils::_runBenchmarking("write");
        timer.start();

        result = writer.importFromFile(fileName);
        qInfo() << "write............." << timer.elapsed() << "ms";
        Utils::_endBenchmarking("write", sWrite);
    }
    if(!result)
    {
        qWarning() << "cannot write data to db";
    }

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
}

