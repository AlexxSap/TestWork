#include "TestSaleHistoryWriter.h"

TestSaleHistoryWriter::TestSaleHistoryWriter(QObject *parent) : QObject(parent)
{

}

template <typename T>
bool compareLists(const QList<T> &act, const QList<T> &exp)
{
    if(act.count() != exp.count())
    {
        return false;
    }

    for(int i = 0; i < act.count(); i++)
    {
        if(act.at(i) != exp.at(i))
        {
            return false;
        }
    }
    return true;
}

void TestSaleHistoryWriter::removeFile(const QString &fileName)
{
    if(QFile::exists(fileName))
    {
        if(!QFile::remove(fileName))
        {
            qWarning() << "Не удалось удалить тестовый файл" << fileName;
            QVERIFY(false);
        }
    }
}

void TestSaleHistoryWriter::createTestDB(const QString &dbName)
{
    const QString connName("testDB");
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connName);
        db.setDatabaseName(dbName);
        if(!db.open())
        {
            qWarning() << "Не удалось создать тестовую БД";
            QVERIFY(false);
        }
        db.close();

        if(!db.open())
        {
            qWarning() << "Тестовая БД не доступна";
            QVERIFY(false);
        }

        db.transaction();

        if(!createTestDbStructure(db))
        {
            db.rollback();
            qWarning() << "Не удалось создать структуру таблиц в тестовой БД";
            QVERIFY(false);
        }
        db.commit();
        db.close();
    }
    QSqlDatabase::removeDatabase(connName);
}

bool TestSaleHistoryWriter::createTestDbStructure(QSqlDatabase &db)
{
    QSqlQuery query(db);
    bool res = query.exec("create table t_items("
                          "f_id integer primary key asc, "
                          "f_product text not null, "
                          "f_storage text not null"
                          ");");
    if (!res)
    {
        return false;
    }

    res = query.exec("create table t_datas("
                     "f_item references t_items(f_id) on delete cascade, "
                     "f_date real not null, "
                     "f_sold real not null, "
                     "f_rest real not null, "
                     "primary key(f_item, f_date)"
                     ");");
    return res;
}

void TestSaleHistoryWriter::testSaleHistoryWriter()
{
    QFETCH(QString, dbName);
    QFETCH(QStringList, raw);

    QFETCH(bool, expValid);
    QFETCH(QStringList, expProductList);
    QFETCH(QStringList, expStorageList);
    QFETCH(QList<QDate>, expDateList);
    QFETCH(QList<double>, expSoldList);
    QFETCH(QList<double>, expRestList);

    removeFile(dbName);
    createTestDB(dbName);

    SaleHistoryParser parser;
    QList<SaleHistoryDay> list = parser.parse(raw);
    bool actValid = parser.isValid();

    QStringList actProductList;
    QStringList actStorageList;
    QList<QDate> actDateList;
    QList<double> actSoldList;
    QList<double> actRestList;

    if(actValid)
    {
        bool result = false;
        {
            SaleHistoryWriter writer(dbName);
            result = writer.write(list);
        }

        if(!result)
        {
            removeFile(dbName);
            qWarning() << "Не удалось записать данные в БД";
            QVERIFY(false);
        }

        {
            SqliteAllDataDataSelector request(dbName);
            QSqlQuery query = request.get();
            if(!query.first())
            {
                removeFile(dbName);
                qWarning() << "Не удалось считать данные из БД";
                QVERIFY(false);
            }

            do
            {
                actProductList.append(query.value(0).toString());
                actStorageList.append(query.value(1).toString());
                actDateList.append(query.value(2).toDate());
                actSoldList.append(query.value(3).toDouble());
                actRestList.append(query.value(4).toDouble());
            } while(query.next());
        }
    }
    removeFile(dbName);

    QVERIFY(expValid == actValid);
    QVERIFY(compareLists(expProductList, actProductList));
    QVERIFY(compareLists(expStorageList, actStorageList));
    QVERIFY(compareLists(expDateList, actDateList));
    QVERIFY(compareLists(expSoldList, actSoldList));
    QVERIFY(compareLists(expRestList, actRestList));
}

void TestSaleHistoryWriter::testSaleHistoryWriter_data()
{
    QTest::addColumn<QString>("dbName");
    QTest::addColumn<QStringList>("raw");

    QTest::addColumn<bool>("expValid");
    QTest::addColumn<QStringList>("expProductList");
    QTest::addColumn<QStringList>("expStorageList");
    QTest::addColumn<QList<QDate> >("expDateList");
    QTest::addColumn<QList<double> >("expSoldList");
    QTest::addColumn<QList<double> >("expRestList");

    QTest::newRow("good data") << QString("good.db")
                               << (QStringList() << "товар1;склад1;2015.08.10;50.0;20.0"
                                   << "товар1;склад1;2015.08.11;20.0;10.0"
                                   << "товар2;склад1;2015.08.09;220.0;11.0"
                                   << "товар2;склад2;2015.08.10;2.0;1.0")

                               << true
                               << (QStringList() << "товар1"
                                   << "товар1"
                                   << "товар2"
                                   << "товар2")
                               << (QStringList() << "склад1"
                                   << "склад1"
                                   << "склад1"
                                   << "склад2")
                               << (QList<QDate>() << QDate(2015, 8, 10)
                                   << QDate(2015, 8, 11)
                                   << QDate(2015, 8, 9)
                                   << QDate(2015, 8, 10))
                               << (QList<double>() << 50.0 << 20.0 << 220.0 << 2.0)
                               << (QList<double>() << 20.0 << 10.0 << 11.0 << 1.0);

    QTest::newRow("bad data") << QString("bad.db")
                              << (QStringList() << "товар1;склад1;2015.08.10;50.0;20.0"
                                  << "товар1;склад1;2015.08.11;20.0;10.0"
                                  << "товар2;склад1;2015.08.09;220.0;11.0"
                                  << "товар2;склад2;2015.08.10;1.0")

                              << false
                              << QStringList()
                              << QStringList()
                              << QList<QDate>()
                              << QList<double>()
                              << QList<double>();
}

