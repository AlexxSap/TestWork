#include "TestWriteReadFileToDB.h"

TestWriteReadFileToDB::TestWriteReadFileToDB(QObject *parent) : QObject(parent){
    _testInFileName="testIn.csv";
    _testOutFileName="testOut.csv";
    _testDBName="test.db";
}

void TestWriteReadFileToDB::createFile(const QStringList &data){
    QFile file(_testInFileName);
    if(file.open(QIODevice::WriteOnly)){
        foreach (QString temp, data) {
            file.write(QString(temp+"\r\n").toLocal8Bit());
        }
        file.close();
    }
}

void TestWriteReadFileToDB::removeFiles(){
    if(QFile::exists(_testInFileName))
        QFile::remove(_testInFileName);

    if(QFile::exists(_testDBName))
        QFile::remove(_testDBName);

    if(QFile::exists(_testOutFileName))
        QFile::remove(_testOutFileName);
}

bool TestWriteReadFileToDB::createTestDB()
{
    QString connName("testDB");
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connName);
        db.setDatabaseName(_testDBName);
        if(!db.open()){
            qDebug() << "can't create db";
            return false;
        }
        db.close();

        if(!db.open()){
            qDebug() << "db not opened";
            return false;
        }

        db.transaction();
        QSqlQuery query(db);
        if(!query.exec("create table t_goods("
                       "f_name text not null, "
                       "f_date real not null, "
                       "f_sold real not null, "
                       "f_rest real not null, "
                       "primary key(f_name, f_date)"
                       ");")){
            db.rollback();
            qDebug() << query.lastError().text();
            return false;
        }
        db.commit();
        db.close();
    }
    QSqlDatabase::removeDatabase(connName);
    return true;
}

bool TestWriteReadFileToDB::compData(const QStringList &act, const QStringList &exp){
    if(act.count()!=exp.count())
        return false;

    for(int i=0;i<act.count();i++){
        if(QString::localeAwareCompare(act.at(i),exp.at(i))!=0)
            return false;
    }
    return true;
}

bool TestWriteReadFileToDB::compError(const int &act, const int &exp){
    return act==exp;
}

QStringList TestWriteReadFileToDB::readDataFromFile(const QString &fileName){
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return QStringList();

    if(file.size()==0)
        return QStringList();

    file.reset();
    QStringList lst;
    while(!file.atEnd())
        lst << QString(file.readLine().trimmed());

    file.close();
    return lst;
}

void TestWriteReadFileToDB::writeReadFileToDB()
{
    QFETCH(QStringList, data);
    QFETCH(QString, request);
    QFETCH(int, expectedReaction);
    QFETCH(QStringList, expectedData);

    ///notes для каждого data должне быть свой собственный файл, чтобы в определённых случаяъ после тестов можно было посмотреть
    ///notes что там в нём понаписано

    ///notes для создания тестовых файло и баз обычно используем макросы, что бы чётко было видно, в какой строчке тестов тест упал.
    removeFiles();
    createFile(data);
    createTestDB();

    SqliteDataBase db(_testDBName);
    CsvFileReader csvFR;
    CsvFileWriter csvFW;

    int actualReaction=static_cast<int>(csvFR.readFromFile(_testInFileName, db));
    bool isWriteToFile=csvFW.writeToFileAllData(_testOutFileName,db);
    QStringList actualData=readDataFromFile(_testOutFileName);

    removeFiles();

    bool isWriteToDB=compError(actualReaction,expectedReaction);
    bool isDatasEqual=compData(actualData,expectedData);

    if(!(isWriteToDB && isWriteToFile && isDatasEqual)){
        QVERIFY(false);

        ///notes лучше писать сразу в qWarning. например, я вообще дебаг не собираю никогда и не увижу таких сообщений.

        qDebug() << "Реакции при записи в БД совпадают?" << isWriteToDB;
        qDebug() << "Данные записались в файл? " << isWriteToFile;
        qDebug() << "Данные совпадают?" << isDatasEqual;
    }
    else
        QVERIFY(true);
}

void TestWriteReadFileToDB::writeReadFileToDB_data()
{
    QTest::addColumn<QStringList>("data");
    QTest::addColumn<QString>("request");
    QTest::addColumn<int>("expectedReaction");
    QTest::addColumn<QStringList>("expectedData");

    QTest::newRow("empty file") << QStringList()
                                << ""
                                << static_cast<int>(FileReader::EmptyFile)
                                << QStringList();

    QTest::newRow("correct simple file") << (QStringList() << "good1;2015.07.29;12.1;544.5"
                                             << "good2;2015.07.29;23.6;54.1"
                                             << "good2;2015.07.30;2;5.4")
                                         << ""
                                         << static_cast<int>(FileReader::NoError)
                                         << (QStringList() << "good1;2015.07.29;12.1;544.5"
                                             << "good2;2015.07.29;23.6;54.1"
                                             << "good2;2015.07.30;2;5.4");
}
