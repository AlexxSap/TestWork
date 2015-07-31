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
        file.write(QString(temp+"\r\n").toLocal8Bit());
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

bool TestWriteReadFileToDB::createTestDbStructure(QSqlDatabase &db)
{
    QSqlQuery query(db);
    bool res=query.exec("create table t_products("
                        "f_id integer primary key asc, "
                        "f_name text not null"
                        ");");
    if (!res)
    {
        return false;
    }

    res=query.exec("create table t_datas("
                           "f_product references t_products(f_id) on delete cascade, "
                           "f_date real not null, "
                           "f_sold real not null, "
                           "f_rest real not null, "
                           "primary key(f_product, f_date)"
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

bool TestWriteReadFileToDB::compData(const QStringList &act, const QStringList &exp)
{
    if(act.count()!=exp.count())
    {
        return false;
    }

    for(int i=0;i<act.count();i++)
    {
        if(QString::localeAwareCompare(act.at(i),exp.at(i))!=0)
        {
            return false;
        }
    }
    return true;
}

bool TestWriteReadFileToDB::compError(const int &act, const int &exp)
{
    return act==exp;
}

QStringList TestWriteReadFileToDB::readDataFromFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        return QStringList();
    }

    if(file.size()==0)
    {
        return QStringList();
    }

    file.reset();
    QStringList lst;
    while(!file.atEnd())
    {
        lst << QString(file.readLine().trimmed());
    }

    file.close();
    return lst;
}

void TestWriteReadFileToDB::testWriteReadFileToDB()
{
    QFETCH(QString, inFileName);
    QFETCH(QString, outFileName);
    QFETCH(QString, dbName);
    QFETCH(QStringList, data);
    QFETCH(QString, request);
    QFETCH(int, expectedReaction);
    QFETCH(QStringList, expectedData);


    ///notes для создания тестовых файло и баз обычно используем макросы,
    /// что бы чётко было видно, в какой строчке тестов тест упал.
    removeFile(inFileName);
    removeFile(outFileName);
    removeFile(dbName);

    createFile(inFileName, data);
    createTestDB(dbName);

    SqliteDataBase db(dbName);
    CsvFileReader csvFR;
    CsvFileWriter csvFW;

    const int actualReaction=static_cast<int>(csvFR.readFromFile(inFileName, db));
    const bool isWriteToFile=csvFW.writeToFileAllData(outFileName,db);
    QStringList actualData=readDataFromFile(outFileName);

    removeFile(inFileName);
    removeFile(outFileName);
    removeFile(dbName);

    const bool isWriteToDB=compError(actualReaction,expectedReaction);
    const bool isDatasEqual=compData(actualData,expectedData);

    if(!(isWriteToDB && isWriteToFile && isDatasEqual))
    {
        qWarning() << "Реакции при записи в БД совпадают?" << isWriteToDB;
        qWarning() << "Данные записались в файл? " << isWriteToFile;
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
    QTest::addColumn<QString>("request");
    QTest::addColumn<int>("expectedReaction");
    QTest::addColumn<QStringList>("expectedData");

    QTest::newRow("empty file") << QString("inFileEmpty.csv")
                                << QString("outFileEmpty.csv")
                                << QString("dbEmpty.db")
                                << QStringList()
                                << QString()
                                << static_cast<int>(FileReader::EmptyFile)
                                << QStringList();

    QTest::newRow("correct simple file") << QString("inFileCorrect.csv")
                                         << QString("outFileCorrect.csv")
                                         << QString("dbCorrect.db")
                                         << (QStringList() << "товар1_?;2015.07.29;12.1;544.5"
                                             << "product2;2015.07.29;23.6;54.1"
                                             << "product2;2015.07.30;2;5.4")
                                         << ""
                                         << static_cast<int>(FileReader::NoError)
                                         << (QStringList() << "товар1;2015.07.29;12.1;544.5"
                                             << "product2;2015.07.29;23.6;54.1"
                                             << "product2;2015.07.30;2;5.4");
}
