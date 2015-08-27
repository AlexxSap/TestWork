#include "BenchmarkWriteReadWithAnalogs.h"

void BenchmarkWriteReadWithAnalogs::run(const int &days,
                                        const int &storages,
                                        const int &products,
                                        const int &analogsGroupNum,
                                        const int &analogsInGroupNum)
{
    const QString dbName(QString::number(analogsGroupNum) + "_"
                         + QString::number(analogsInGroupNum) + "_"
                         + "_BAR_TestDBase.db");

    const QString fileName(QString::number(analogsGroupNum) + "_"
                           + QString::number(analogsInGroupNum) + "_"
                           + "_BAR_TestFile.csv");

    QThread::msleep(100);
    qInfo() << "-------Benchmark for write and read saleshistory witg analogs-------";
    qInfo() << days << " days, "
            << storages << " storages, "
            << products << " products, "
            << analogsGroupNum << " analogsGroupNum, "
            << analogsInGroupNum << " analogsInGroupNum";

    if(!TestUtility::removeFile(dbName))
    {
        qWarning() << "cannot remove test-db in begin of benchmark";
        return;
    }
    if(!TestUtility::removeFile(fileName))
    {
        qWarning() << "cannot remove test-file in begin of benchmark";
        return;
    }

    {
        AnalogsTable table = AnalogsTableGenerator::generateTable(analogsGroupNum,
                                                                  analogsInGroupNum);

        AnalogsWriter writer(dbName);
        if(!writer.write(table))
        {
            qWarning() << "cannot write AnalogsTable to db";
            return;
        }

    }








    if(!TestUtility::removeFile(dbName))
    {
        qWarning() << "cannot remove test-db in end of benchmark";
        return;
    }

    if(!TestUtility::removeFile(fileName))
    {
        qWarning() << "cannot remove test-file in end of benchmark";
        return;
    }
}
