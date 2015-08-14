#include "TestUtility.h"

bool TestUtility::createTestDbStructure(QSqlDatabase &db)
{
    QSqlQuery query(db);
    bool res = query.exec("create table t_datas("
                          "f_storage text not null, "
                          "f_product text not null, "
                          "f_date real not null, "
                          "f_sold real not null, "
                          "f_rest real not null, "
                          "primary key(f_storage, f_product, f_date));");
    return res;
}

bool TestUtility::createFile(const QString &fileName, const QStringList &data)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    foreach (const QString &temp, data)
    {
        file.write(QString(temp + "\r\n").toLocal8Bit());
    }
    file.close();
    return true;
}

bool TestUtility::removeFile(const QString &fileName)
{
    if(QFile::exists(fileName))
    {
        if(!QFile::remove(fileName))
        {
            return false;
        }
    }
    QThread::msleep(100); // просто винда не всегда успевает удалить файл
    return true;
}

bool TestUtility::createTestDB(const QString &dbName)
{
    const QString connName("testDB");
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connName);
        db.setDatabaseName(dbName);
        if(!db.open())
        {
            return false;
        }
        db.close();

        if(!db.open())
        {
            return false;
        }

        db.transaction();

        if(!createTestDbStructure(db))
        {
            db.rollback();
            return false;
        }
        db.commit();
        db.close();
    }
    QSqlDatabase::removeDatabase(connName);
    return true;
}

bool TestUtility::compareListWithoutOrder(const QStringList &act, const QStringList &exp)
{
    if(act.count() != exp.count())
    {
        return false;
    }
    foreach (const QString &str, exp)
    {
        if(!act.contains(str))
        {
            return false;
        }
    }
    return true;
}
