#include "TestWriteReadFileToDB.h"

TestWriteReadFileToDB::TestWriteReadFileToDB(QObject *parent)
    : QObject(parent)
{

}

void TestWriteReadFileToDB::createFile(const QString &fileName, const QStringList &data)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "Не удалось создать тестовый файл" << fileName;
        QVERIFY(false);
    }

    foreach (QString temp, data)
    {
        file.write(QString(temp + "\r\n").toLocal8Bit());
    }
    file.close();
}

void TestWriteReadFileToDB::removeFile(const QString &fileName)
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

///notes если тесту нужно напрямую что-то создавать в базе, то это очень исключительная ситуация
/// (у нас, например, используется такое для проверки обратной совместимости новых версий программ со старыми)
bool TestWriteReadFileToDB::createTestDbStructure(QSqlDatabase &db)
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


void TestWriteReadFileToDB::createTestDB(const QString &dbName)
{
    const QString connName("testDB");
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connName);
        db.setDatabaseName(dbName);
        if(!db.open())
        {
            ///notes обычно в тестах всю отладочную печать делаем латинскими символами, что бы избежать всяких неприятных ситуаций с utf-8
            qWarning() << "Не удалось создать тестовую БД";

            ///notes для таких вещей обычно используют QFAIL
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

bool TestWriteReadFileToDB::compData(const QStringList &act, const QStringList &exp)
{
    if(act.count() != exp.count())
    {
        return false;
    }
    foreach (QString str, exp)
    {
        if(!act.contains(str))
        {
            return false;
        }
    }
    return true;
}

bool TestWriteReadFileToDB::compError(const int &act, const int &exp)
{
    return act == exp;
}

QStringList TestWriteReadFileToDB::readDataFromFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        return QStringList();
    }

    if(file.size() == 0)
    {
        return QStringList();
    }

    QTextStream ts(&file);
    ts.setCodec(QTextCodec::codecForName("Windows-1251"));
    QStringList lst;
    while(!ts.atEnd())
    {
        lst << ts.readLine().trimmed();
    }

    file.close();
    return lst;
}

 ///notes для проверки такой примитивных операции, как запись и чтение, тесты выглядят очень сложно.
 /// в идеале тест должен быть линейным кодом(без условий и циклов),
 /// чтобы минимально снизить риск ошибки в тестах(тесты на тесты писать не очень прилично).
 /// возможно, в тесте тестируется что-то ещё, и это что-то ещё заслуживает тогда отдельного теста.

void TestWriteReadFileToDB::testWriteReadFileToDB()
{
    ///notes насколько критично в ручную задавать имена файлов? почему их нельзя генерировать прямо в теле теста?
    QFETCH(QString, inFileName);
    QFETCH(QString, outFileName);
    QFETCH(QString, dbName);
    QFETCH(QStringList, data);
    QFETCH(QString, selectionCase);
    QFETCH(int, expectedReaction);
    QFETCH(bool, expectedWritedToFile);
    QFETCH(QStringList, expectedData);

    ///notes для создания тестовых файло и баз обычно используем макросы,
    /// что бы чётко было видно, в какой строчке тестов тест упал.
    removeFile(inFileName);
    removeFile(outFileName);
    removeFile(dbName);

    createFile(inFileName, data);
    createTestDB(dbName);

    CsvFileReader fileReader;
    CsvFileWriter fileWriter;

    int actualReaction = 0;
    {
        DataBase db(dbName);
        actualReaction = static_cast<int>(fileReader.readFromFile(inFileName, db));
    }

    bool actualWritedToFile = false;
    {
        if(selectionCase.isEmpty())
        {
            SqliteAllDataDataSelector request(dbName);
            actualWritedToFile = fileWriter.writeToFile(outFileName,request);
        }
        else
        {
            QString rxPattern = QString("([?а-яА-ЯёЁa-zA-Z0-9_!]+)|"
                                        "([0-9]{4}\.(0[1-9]|1[012])\.(0[1-9]|1[0-9]|2[0-9]|3[01]))");
            const QRegExp rx(rxPattern);

            if(rx.exactMatch(selectionCase))
            {
                const QString product = rx.cap(1); ;
                const QString date = rx.cap(2);
                if(!product.isEmpty())
                {
                    SqliteSelByProductDataSelector request(dbName);
                    request.setCaseValue(product);
                    actualWritedToFile = fileWriter.writeToFile(outFileName,request);
                }
                else if(!date.isEmpty())
                {
                    SqliteSelByDateDataSelector request(dbName);
                    request.setCaseValue(date);
                    actualWritedToFile = fileWriter.writeToFile(outFileName,request);
                }
            }

        }
    }
    QStringList actualData = readDataFromFile(outFileName);

    removeFile(inFileName);
    removeFile(outFileName);
    removeFile(dbName);

    const bool isWriteToDB = compError(actualReaction,expectedReaction);
    const bool isWritedToFile = (actualWritedToFile == expectedWritedToFile);
    const bool isDatasEqual = compData(actualData,expectedData);

    if(!(isWriteToDB && isWritedToFile && isDatasEqual))
    {
        qWarning() << "Реакции при записи в БД совпадают?" << isWriteToDB;
        qWarning() << "Реакции записи в файл совпадают? " << isWritedToFile;
        qWarning() << "Данные совпадают?" << isDatasEqual;
        QVERIFY(false);
    }
    else
    {
        QVERIFY(true);
    }
}

void TestWriteReadFileToDB::testWriteReadFileToDB_data()
{
    QTest::addColumn<QString>("inFileName");
    QTest::addColumn<QString>("outFileName");
    QTest::addColumn<QString>("dbName");
    QTest::addColumn<QStringList>("data");
    QTest::addColumn<QString>("selectionCase");

    ///notes почему тут используется int, а не FileReader::Error ?
    QTest::addColumn<int>("expectedReaction");
    QTest::addColumn<bool>("expectedWritedToFile");
    QTest::addColumn<QStringList>("expectedData");

    QTest::newRow("empty file") << QString("inFileEmpty.csv")
                                << QString("outFileEmpty.csv")
                                << QString("dbEmpty.db")
                                << QStringList()
                                << ""
                                << static_cast<int>(FileReader::EmptyFile)
                                << false
                                << QStringList();

    QTest::newRow("correct simple file") << QString("inFileCorrect.csv")
                                         << QString("outFileCorrect.csv")
                                         << QString("dbCorrect.db")
                                         << (QStringList() << "\"тов;ар1!\";склад_1;2015.07.29;12.10;544.50"
                                             << "product2;storage1;2015.07.29;23.60;54.10"
                                             << "product2;storage1;2015.07.30;2.00;5.40")
                                         << ""
                                         << static_cast<int>(FileReader::NoError)
                                         << true
                                         << (QStringList() << "\"тов;ар1!\";склад_1;2015.07.29;12.10;544.50"
                                             << "product2;storage1;2015.07.29;23.60;54.10"
                                             << "product2;storage1;2015.07.30;2.00;5.40");

    QTest::newRow("file with ugly date") << QString("inUglyDate.csv")
                                         << QString("outUglyDate.csv")
                                         << QString("dbUglyDate.db")
                                         << (QStringList() << "product1;storage1;2015.09.41;3;54.5"
                                             << "product2;storage1;29.12.2015;23.6;5.1"
                                             << "товар4;storage1;2015.27.30;2;5.4")
                                         << ""
                                         << static_cast<int>(FileReader::FileNotLoaded)
                                         << false
                                         << QStringList();

    QTest::newRow("file with uncorrect date") << QString("inUncDate.csv")
                                              << QString("outUncDate.csv")
                                              << QString("dbUncDate.db")
                                              << (QStringList() << "product1;storage1;2015.09.31;12.1;544.5"
                                                  << "product_2!;storage1;2015.07.29;23.6;54.1"
                                                  << "product2storage1;;2015.02.30;2;5.4")
                                              << ""
                                              << static_cast<int>(FileReader::FileNotLoaded)
                                              << false
                                              << QStringList();

    QTest::newRow("file with a negative number of product") << QString("inNegative.csv")
                                                            << QString("outNegative.csv")
                                                            << QString("dbNegative.db")
                                                            << (QStringList() << "product1;storage1;2015.07.20;-3;54.5"
                                                                << "товар2;storage1;2015.07.20;23.6;-5.1"
                                                                << "product4;storage1;2015.07.00;2;5.4")
                                                            << ""
                                                            << static_cast<int>(FileReader::FileNotLoaded)
                                                            << false
                                                            << QStringList();

    QTest::newRow("file with a different structure") << QString("inDifferent.csv")
                                                     << QString("outDifferent.csv")
                                                     << QString("dbDifferent.db")
                                                     << (QStringList() << "product1;storage1;20.07.2015;.3;54.5;45"
                                                         << "product2;something else;2015.07.20,23.6;.5.1"
                                                         << "56.5,wtf")
                                                     << ""
                                                     << static_cast<int>(FileReader::FileNotLoaded)
                                                     << false
                                                     << QStringList();

    QTest::newRow("get by product") << QString("inFileProduct.csv")
                                    << QString("outFileProduct.csv")
                                    << QString("dbProduct.db")
                                    << (QStringList() << "товар1;storage1;2015.07.29;12.10;544.50"
                                        << "product2;storage1;2015.07.29;23.60;54.10"
                                        << "product2;storage2;2015.07.30;2.00;5.40"
                                        << "товар1;склад2;2015.07.28;5.03;1.40")
                                    << "товар1"
                                    << static_cast<int>(FileReader::NoError)
                                    << true
                                    << (QStringList() << "товар1;storage1;2015.07.29;12.10;544.50"
                                        << "товар1;склад2;2015.07.28;5.03;1.40");

    QTest::newRow("get by date") << QString("inFileDate.csv")
                                 << QString("outFileDate.csv")
                                 << QString("dbDate.db")
                                 << (QStringList() << "товар1;storage1;2015.07.29;12.10;544.50"
                                     << "product2;storage1;2015.07.29;23.60;54.10"
                                     << "product2;storage1;2015.07.30;2.00;5.40"
                                     << "товар1;storage1;2015.07.30;5.03;1.40")
                                 << "2015.07.30"
                                 << static_cast<int>(FileReader::NoError)
                                 << true
                                 << (QStringList() << "product2;storage1;2015.07.30;2.00;5.40"
                                     << "товар1;storage1;2015.07.30;5.03;1.40");
}
