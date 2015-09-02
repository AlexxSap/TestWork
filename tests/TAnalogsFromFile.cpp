#include "TAnalogsFromFile.h"

TAnalogsFromFile::TAnalogsFromFile(QObject *parent) : QObject(parent)
{

}

void TAnalogsFromFile::TestImportFromFile()
{
    QFETCH(QStringList, fileData);
    QFETCH(QList<ID>, idList);
    QFETCH(AnalogsTable, expResult);

    const QString dbName(QString(QTest::currentDataTag()) + "tdb");
    const QString fileName(QString(QTest::currentDataTag()) + "tf.csv");

    DataBaseInfo info;
    info.setDataBaseName(dbName);
    info.setUserName("root");
    info.setPassword("1234");

    if(!DataBase::remDataBase(info))
    {
        QFAIL("cannot remove test-db in begin of test");
    }

    if(!TestUtility::removeFile(fileName))
    {
        QFAIL("cannot remove test-file in begin of test");
    }

    if(!TestUtility::createFile(fileName, fileData))
    {
        QFAIL("cannot create test-file");
    }

    bool isWrited = false;
    {
        AnalogsWriter writer(info);
        isWrited = writer.importFromFile(fileName);
    }

    if(!isWrited)
    {
        DataBase::remDataBase(info);
        QFAIL("cannot write data from file");
    }

    AnalogsTable actResult;
    {
        AnalogsReader reader(info);
        actResult = reader.fetch(idList);
    }

    if(!DataBase::remDataBase(info))
    {
        QFAIL("cannot remove test-db in end of test");
    }

    if(!TestUtility::removeFile(fileName))
    {
        QFAIL("cannot remove test-file in end of test");
    }

    QCOMPARE(actResult, expResult);
}

void TAnalogsFromFile::TestImportFromFile_data()
{
    QTest::addColumn<QStringList>("fileData");
    QTest::addColumn<QList<ID> >("idList");
    QTest::addColumn<AnalogsTable>("expResult");

    QTest::newRow("empty") << QStringList()
                            << (QList<ID>()
                                << "prod03"
                                << "prod11")
                            << AnalogsTable();

    QTest::newRow("simple") << (QStringList()
                                << "prod01;prod02;прод03"
                                << "prod11_чавой?"
                                << "prod21;prod22;prod23;prod24")
                            << (QList<ID>()
                                << "прод03"
                                << "prod11_чавой?")
                            << (AnalogsTable()
                                << (Analogs("prod01")
                                    << ID("prod02")
                                    << ID("прод03"))
                                << Analogs("prod11_чавой?"));

    QTest::newRow("empty_str") << (QStringList()
                                << "prod01;prod02;прод03"
                                << "prod11"
                                << ""
                                << "prod21;prod22;prod23;prod24")
                            << (QList<ID>()
                                << "прод03"
                                << "prod11")
                            << (AnalogsTable()
                                << (Analogs("prod01")
                                    << ID("prod02")
                                    << ID("прод03"))
                                << Analogs("prod11"));

    QTest::newRow("id_not_found") << (QStringList()
                                << "prod01;prod02;prod03"
                                << "prod11"
                                << "prod21;prod22;prod23;prod24")
                            << (QList<ID>()
                                << "prod03"
                                << "prod41")
                            << (AnalogsTable()
                                << (Analogs("prod01")
                                    << ID("prod02")
                                    << ID("prod03")));
}

