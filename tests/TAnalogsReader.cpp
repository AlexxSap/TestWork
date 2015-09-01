#include "TAnalogsReader.h"

TAnalogsReader::TAnalogsReader(QObject *parent) : QObject(parent)
{

}

void TAnalogsReader::TestReadAnalogs()
{
    QFETCH(AnalogsTable, table);
    QFETCH(QList<ID>, IdList);
    QFETCH(AnalogsTable, expResult);

    const QString dbName(QString(QTest::currentDataTag()) + "tdb");
    DataBaseInfo info;
    info.setDataBaseName(dbName);
    info.setUserName("root");
    info.setPassword("1234");

    if(!TestUtility::removeFile(info.dataBaseName()))
    {
        QFAIL("cannot remove test-db in begin of test");
    }

    bool isWrited = false;
    {
        AnalogsWriter writer(info);
        isWrited = writer.write(table);
    }

    if(!isWrited)
    {
        TestUtility::removeFile(info.dataBaseName());
        QFAIL("cannot write data to db");
    }

    AnalogsTable actResult;
    {
        AnalogsReader reader(info);
        actResult = reader.fetch(IdList);
    }

    if(!TestUtility::removeFile(info.dataBaseName()))
    {
        QFAIL("cannot remove test-db in end of test");
    }

    QCOMPARE(actResult, expResult);
}

void TAnalogsReader::TestReadAnalogs_data()
{
    QTest::addColumn<AnalogsTable>("table");
    QTest::addColumn< QList<ID> >("IdList");
    QTest::addColumn<AnalogsTable>("expResult");

    QTest::newRow("simple") << (AnalogsTable()
                                << (Analogs("p01")
                                    << ID("прод02")
                                    << ID("p03"))
                                << (Analogs("p11")
                                    << ID("p12"))
                                << (Analogs("p21")
                                    << ID("p22")
                                    << ID("p23")))
                            << (QList<ID>()
                                << "p12"
                                << "прод02")
                            << (AnalogsTable()
                                << (Analogs("p11")
                                    << ID("p12"))
                                << (Analogs("p01")
                                    << ID("прод02")
                                    << ID("p03")));

    QTest::newRow("empty") << (AnalogsTable()
                               << (Analogs("p01")
                                   << ID("p02")
                                   << ID("p03"))
                               << (Analogs("p11")
                                   << ID("p12"))
                               << (Analogs("p21")
                                   << ID("p22")
                                   << ID("p23")))
                           << (QList<ID>()
                               << "p42")
                           << AnalogsTable();

    QTest::newRow("combine") << (AnalogsTable()
                                 << (Analogs("p01")
                                     << ID("p02")
                                     << ID("p03"))
                                 << (Analogs("p11")
                                     << ID("p12"))
                                 << (Analogs("p21")
                                     << ID("p22")
                                     << ID("p23")))
                             << (QList<ID>()
                                 << "p42"
                                 << "p02")
                             << (AnalogsTable()
                                 << (Analogs("p01")
                                     << ID("p02")
                                     << ID("p03")));

    QTest::newRow("multiple") << (AnalogsTable()
                                  << (Analogs("p01")
                                      << ID("p02")
                                      << ID("p03"))
                                  << (Analogs("p11")
                                      << ID("p12"))
                                  << (Analogs("p21")
                                      << ID("p02")
                                      << ID("p23")))
                              << (QList<ID>()
                                  << "p02")
                              << (AnalogsTable()
                                  << (Analogs("p01")
                                      << ID("p02")
                                      << ID("p03"))
                                  << (Analogs("p21")
                                      << ID("p02")
                                      << ID("p23")));

    QTest::newRow("multiple2") << (AnalogsTable()
                                   << (Analogs("p01")
                                       << ID("p02")
                                       << ID("p03"))
                                   << (Analogs("p01")
                                       << ID("p12"))
                                   << (Analogs("p21")
                                       << ID("p22")
                                       << ID("p23")))
                               << (QList<ID>()
                                   << "p01")
                               << (AnalogsTable()
                                   << (Analogs("p01")
                                       << ID("p02")
                                       << ID("p03")
                                       << ID("p12")));

}
