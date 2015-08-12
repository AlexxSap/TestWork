#include "TestSalesHistoryStreamReader.h"

///notes чем не устроил стандартный оператор равенства списков?
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

TestSalesHistoryStreamReader::TestSalesHistoryStreamReader(QObject *parent) : QObject(parent)
{

}

void TestSalesHistoryStreamReader::removeFile(const QString &fileName)
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

void TestSalesHistoryStreamReader::createTestDB(const QString &dbName)
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

bool TestSalesHistoryStreamReader::createTestDbStructure(QSqlDatabase &db)
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

///notes я бы использовал такой набор тестовых данных:
/// QList<SaleHistoryDay> - что пишем в базу(изначальное состояние базы то основного тестирования)
/// QList<Item> - для чего читаем
/// QDate - откуда читаем
/// QDate - докуда читаем
/// QList<SaleHistory> - что должно прочитаться
///
/// текущий набор тестовых данных будет неудобен, например, если что-то поменяется в
/// формате загрузки истории продаж. Читалке истории продаж абсолютно всё равно на формат истории продаж,
/// её интересуют только уже записанные каким-либо образом продажи в базу. а в этом тесте создаётся такая вот зависимость.

void TestSalesHistoryStreamReader::testSalesHistoryStreamReader()
{
    QFETCH(QString, dbName);
    QFETCH(QStringList, raw);

    QFETCH(QStringList, caseStorage);
    QFETCH(QStringList, caseProduct);
    QFETCH(QDate, caseDateFrom);
    QFETCH(QDate, caseDateTo);

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
            QList<Item> items;
            for(int i = 0; i < caseStorage.count(); i++)
            {
                Item item(caseStorage.at(i), caseProduct.at(i));
                items.append(item);
            }
            SalesHistoryStreamReader reader(items);
            reader.setDbName(dbName);
            bool isOpen = reader.open(caseDateFrom, caseDateTo);
            if(!isOpen)
            {
                removeFile(dbName);
                qWarning() << "Что-то пошло не так при открытии SalesHistoryStreamReader";
                QVERIFY(false);
            }
            do
            {
                SaleHistory history = reader.current();
                if(history.isValid())
                {
                    QList<SaleHistoryDay> days = history.days();
                    foreach (SaleHistoryDay day, days)
                    {
                        actProductList.append(day.item().product());
                        actStorageList.append(day.item().storage());
                        actDateList.append(day.date());
                        actSoldList.append(day.sold());
                        actRestList.append(day.rest());
                    }
                }
            }while (reader.next());
        }
    }
    removeFile(dbName);

    QVERIFY(compareLists(expProductList, actProductList));
    QVERIFY(compareLists(expStorageList, actStorageList));
    QVERIFY(compareLists(expDateList, actDateList));
    QVERIFY(compareLists(expSoldList, actSoldList));
    QVERIFY(compareLists(expRestList, actRestList));
}

void TestSalesHistoryStreamReader::testSalesHistoryStreamReader_data()
{
    QTest::addColumn<QString>("dbName");
    QTest::addColumn<QStringList>("raw");

    QTest::addColumn<QStringList>("caseStorage");
    QTest::addColumn<QStringList>("caseProduct");
    QTest::addColumn<QDate>("caseDateFrom");
    QTest::addColumn<QDate>("caseDateTo");

    QTest::addColumn<QStringList>("expProductList");
    QTest::addColumn<QStringList>("expStorageList");
    QTest::addColumn<QList<QDate> >("expDateList");
    QTest::addColumn<QList<double> >("expSoldList");
    QTest::addColumn<QList<double> >("expRestList");

    QTest::newRow("simple") << QString("simple.db")
                            << (QStringList() << "product1;storage1;2015.08.10;50.0;20.0"
                                << "product1;storage1;2015.08.11;20.0;10.0"
                                << "product1;storage1;2015.08.12;10.0;0.0"
                                << "product2;storage1;2015.08.10;220.0;11.0"
                                << "product2;storage2;2015.08.10;2.0;1.0")

                            << (QStringList() << "storage1" << "storage1")
                            << (QStringList() << "product1" << "product2")
                            << QDate(2015, 8, 10)
                            << QDate(2015, 8, 11)

                            << (QStringList() << "product1"
                                << "product1"
                                << "product2")
                            << (QStringList() << "storage1"
                                << "storage1"
                                << "storage1")
                            << (QList<QDate>() << QDate(2015, 8, 10)
                                << QDate(2015, 8, 11)
                                << QDate(2015, 8, 10))
                            << (QList<double>() << 50.0 << 20.0 << 220.0)
                            << (QList<double>() << 20.0 << 10.0 << 11.0);

    QTest::newRow("simple wo date") << QString("simple2.db")
                                    << (QStringList() << "product1;storage1;2015.08.10;50.0;20.0"
                                        << "product1;storage1;2015.08.11;20.0;10.0"
                                        << "product1;storage1;2015.08.12;10.0;0.0"
                                        << "product2;storage1;2015.08.10;220.0;11.0"
                                        << "product2;storage2;2015.08.10;2.0;1.0")

                                    << (QStringList() << "storage1" << "storage1")
                                    << (QStringList() << "product1" << "product2")
                                    << QDate()
                                    << QDate()

                                    << (QStringList() << "product1"
                                        << "product1"
                                        << "product1"
                                        << "product2")
                                    << (QStringList() << "storage1"
                                        << "storage1"
                                        << "storage1"
                                        << "storage1")
                                    << (QList<QDate>() << QDate(2015, 8, 10)
                                        << QDate(2015, 8, 11)
                                        << QDate(2015, 8, 12)
                                        << QDate(2015, 8, 10))
                                    << (QList<double>() << 50.0 << 20.0 << 10.0 << 220.0)
                                    << (QList<double>() << 20.0 << 10.0 << 0.0 << 11.0);

    QTest::newRow("empty Item") << QString("emptyItem.db")
                                << (QStringList() << "product1;storage1;2015.08.10;50.0;20.0"
                                    << "product1;storage1;2015.08.11;20.0;10.0"
                                    << "product1;storage1;2015.08.12;10.0;0.0"
                                    << "product2;storage1;2015.08.10;220.0;11.0"
                                    << "product2;storage2;2015.08.10;2.0;1.0")

                                << (QStringList() << "storage1" << "storage1")
                                << (QStringList() << "product3" << "product1")
                                << QDate(2015, 8, 10)
                                << QDate(2015, 8, 12)

                                << (QStringList() << "product3"
                                    << "product1"
                                    << "product1"
                                    << "product1")
                                << (QStringList() << "storage1"
                                    << "storage1"
                                    << "storage1"
                                    << "storage1")
                                << (QList<QDate>() << QDate(2015, 8, 10)
                                    << QDate(2015, 8, 10)
                                    << QDate(2015, 8, 11)
                                    << QDate(2015, 8, 12))
                                << (QList<double>() << 0.0 << 50.0 << 20.0 << 10.0)
                                << (QList<double>() << 0.0 << 20.0 << 10.0 << 0.0 );
}
