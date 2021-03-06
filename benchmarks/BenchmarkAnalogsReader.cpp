#include "BenchmarkAnalogsReader.h"

void BenchmarkAnalogsReader::run(const int &groupNum, const int &analogsNum)
{
    const QString dbName(QString::number(groupNum) + "_"
                         + QString::number(analogsNum) + "_"
                         + "_BAR_tdb");

    const QString fileName(QString::number(groupNum) + "_"
                           + QString::number(analogsNum) + "_"
                           + "_BAR_tf.csv");

    DataBaseInfo info;
    info.setDataBaseName(dbName);
    info.setUserName("root");
    info.setPassword("1234");

    QThread::msleep(100);
    qInfo() << "-------Benchmark for write and read analogs-------";
    qInfo() << groupNum << " groupNum, "
            << analogsNum << " analogsNum";

    if(!DataBase::remDataBase(info))
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
                                                                 analogsNum);
        IdList = AnalogsTableGenerator::generateRandomIdList(table);

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
        AnalogsWriter writer(info);
        timer.start();
        isImported = writer.importFromFile(fileName);
        qInfo() << "import from file " << timer.elapsed();
    }

    if(!isImported)
    {
        DataBase::remDataBase(info);
        TestUtility::removeFile(fileName);
        qWarning() << "cannot import from file";
        return;
    }

    {//чтение из базы
        AnalogsReader reader(info);
        timer.start();
        const AnalogsTable table = reader.fetch(IdList);
        Q_UNUSED(table)
        qInfo() << "read from db " << timer.elapsed();
    }

    if(!DataBase::remDataBase(info))
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
