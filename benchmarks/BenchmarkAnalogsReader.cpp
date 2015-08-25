#include "BenchmarkAnalogsReader.h"

void BenchmarkAnalogsReader::run(const int &groupNum, const int &analogsNumMax)
{
    const QString dbName(QString::number(groupNum) + "_"
                         + QString::number(analogsNumMax) + "_"
                         + "TestDBase.db");

    const QString fileName(QString::number(groupNum) + "_"
                           + QString::number(analogsNumMax) + "_"
                           + "TestFile.csv");

    QThread::msleep(100);
    qInfo() << "-------Benchmark for write and read analogs-------";
    qInfo() << groupNum << " groupNum, "
            << analogsNumMax << " analogsNumMax";

    if(!TestUtility::removeFile(dbName))
    {
        qWarning() << "cannot remove test-db in begin of Benchmark";
        return;
    }

    if(!TestUtility::removeFile(fileName))
    {
        qWarning() << "cannot remove test-file in begin of Benchmark";
        return;
    }

    QList<ID> IdList;
    {//запись в файл
        AnalogsTable table = AnalogsTableGenerator::generateTable(groupNum,
                                                                 analogsNumMax);
        IdList = AnalogsTableGenerator::getRandomIdList(table);

        const bool isWritedToFile = CsvFile::write(table, fileName);
        if(!isWritedToFile)
        {
            qWarning() << "cannot write table to file";
            return;
        }
    }

    QElapsedTimer timer;

    bool isImported = false;
    {//импорт из файла
        AnalogsWriter writer(dbName);
        timer.start();
        isImported = writer.importFromFile(fileName);
        qInfo() << "import from file " << timer.elapsed();
    }

    if(!isImported)
    {
        TestUtility::removeFile(dbName);
        TestUtility::removeFile(fileName);
        qWarning() << "cannot import from file";
        return;
    }



    {//чтение из базы
        AnalogsReader reader(dbName);
        timer.start();
        const AnalogsTable table = reader.read(IdList);
        qInfo() << "read from db " << timer.elapsed();
    }

    if(!TestUtility::removeFile(dbName))
    {
        qWarning() << "cannot remove test-db in end of Benchmark";
        return;
    }

    if(!TestUtility::removeFile(fileName))
    {
        qWarning() << "cannot remove test-file in end of Benchmark";
        return;
    }

    qInfo() << "----End of benchmark for write and read analogs---" << endl;
}
