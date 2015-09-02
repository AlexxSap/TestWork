#include "BenchmarkWriteReadWithAnalogs.h"

/*
720, 10, 100, 5, 5
первоначальное 13651  - 3739


*/
void BenchmarkWriteReadWithAnalogs::run(const int &days,
                                        const int &storages,
                                        const int &products,
                                        const int &analogsGroupNum,
                                        const int &analogsInGroupNum)
{
    const QString dbName(QString::number(analogsGroupNum) + "_"
                         + QString::number(analogsInGroupNum) + "_"
                         + "_BARWA_tdb");

    const QString fileName(QString::number(analogsGroupNum) + "_"
                           + QString::number(analogsInGroupNum) + "_"
                           + "_BARWA_tf.csv");

    DataBaseInfo info;
    info.setDataBaseName(dbName);
    info.setUserName("root");
    info.setPassword("1234");

    const Date fromDate = Date(2015, 1, 1);
    const Date toDate = fromDate.addDays(days - 1);

    qint64 writeTime = 0;
    qint64 readTime = 0;

    QThread::msleep(100);
    qInfo() << "-------Benchmark for write and read saleshistory with analogs-------";
    qInfo() << days << " days, "
            << storages << " storages, "
            << products << " products, "
            << analogsGroupNum << " analogsGroupNum, "
            << analogsInGroupNum << " analogsInGroupNum";

    if(!DataBase::remDataBase(info))
    {
        qWarning() << "cannot remove test-db in begin of benchmark";
        return;
    }
    if(!TestUtility::removeFile(fileName))
    {
        qWarning() << "cannot remove test-file in begin of benchmark";
        return;
    }

    QElapsedTimer timer;

    //------запись данных с генератора в БД
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
                DataBase::remDataBase(info);
                TestUtility::removeFile(fileName);
                qWarning() << "cannot write to file";
                return;
            }
        }

        SaleHistoryWriter writer(info);
        timer.start();
        const double sWrite = Utils::_runBenchmarking("write");
        result = writer.importFromFile(fileName);
        Utils::_endBenchmarking("write", sWrite);
        writeTime = timer.elapsed();

        qInfo() << "write............." << writeTime << "ms";
    }
    if(!result)
    {
        DataBase::remDataBase(info);
        TestUtility::removeFile(fileName);
        qWarning() << "cannot write data to db";
        return;
    }

    //------запись аналогов в БД
    if(analogsGroupNum != 0 && analogsInGroupNum != 0)
    {
        AnalogsTable table = AnalogsTableGenerator::generateTable(analogsGroupNum,
                                                                  analogsInGroupNum,
                                                                  products);
//        qInfo() << table;
        AnalogsWriter writer(info);
        if(!writer.write(table))
        {
            qWarning() << "cannot write AnalogsTable to db";
            return;
        }
    }
    qInfo() << "Analogs writed";

    //---чтение из БД
    {
        const QList<Item> items = TestUtility::genRandomItemList(storages, products);
        SalesHistoryStreamReader reader(items, info);
        timer.start();
        bool isOpen = reader.open(Date(), Date());
        const int openTime = timer.elapsed();

        const double sRead = Utils::_runBenchmarking("read");
        timer.start();

                QList<SaleHistory> shList;
        if(isOpen)
        {
            do
            {
                const SaleHistory history = reader.current();
                                shList.append(history);
            } while (reader.next());
        }

        readTime = timer.elapsed() + openTime;
        Utils::_endBenchmarking("read", sRead);
        qInfo() << "read.............."
                << readTime << "ms";

        //-----сравнение результатов
        QList<Item> actList;
        foreach (const SaleHistory &history, shList)
        {
            if(!actList.contains(history.item()))
            {
                actList.append(history.item());
            }
        }
        if(actList.toSet() != items.toSet())
        {
            qWarning() << "item lists not equal";
            qWarning() << "writed list-------";
            qWarning() << items;
            qWarning() << "readed list-------";
            qWarning() << actList;
        }
    }

    if(!DataBase::remDataBase(info))
    {
        qWarning() << "cannot remove test-db in end of benchmark";
        return;
    }

    if(!TestUtility::removeFile(fileName))
    {
        qWarning() << "cannot remove test-file in end of benchmark";
        return;
    }

    qInfo() << "for 10000 products-------";
    writeTime = 10000/products * writeTime;
    readTime = 10000/products * readTime;
    qInfo() << "write " << writeTime << "ms or " << writeTime / 60000 << "min";
    qInfo() << "read " << readTime << "ms or " << readTime / 60000 << "min";

    qInfo() << "----End of benchmark for write and read saleshistory with analogs---" << endl;

}
