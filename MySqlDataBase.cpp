#include "MySqlDataBase.h"

MySqlDataBase::MySqlDataBase()
    : DataBase()
{

}

MySqlDataBase::MySqlDataBase(const DataBaseInfo &info,
                             const QString &connName)
    :DataBase(info, connName)
{
    db_ = QSqlDatabase::addDatabase("QMYSQL", connName);
    db_.setDatabaseName(info.dataBaseName());
    db_.setUserName(info.userName());
    db_.setPassword(info.password());
}

MySqlDataBase::MySqlDataBase(const DataBase &other)
    :DataBase(other)
{
    db_ = QSqlDatabase::addDatabase("QMYSQL",
                                    other.connectionName());
    db_.setDatabaseName(info_.dataBaseName());
    db_.setUserName(info_.userName());
    db_.setPassword(info_.password());
}

MySqlDataBase::~MySqlDataBase()
{

}

bool MySqlDataBase::createEmptyDB()
{
    const QString connName("createEmptyDBMySql");
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", connName);
        db.setHostName(info_.hostName());
        db.setUserName(info_.userName());
        db.setPassword(info_.password());

        if(!db.open())
        {
            return false;
        }
        else
        {
            QSqlQuery query(db);
            QString req("create database if not exists %1;");
            if(!query.prepare(req.arg(info_.dataBaseName())))
            {
                db.close();
                return false;
            }
            if(!query.exec())
            {

                qInfo() << query.lastError().text();
                db.close();
                return false;
            }
            db.setDatabaseName(info_.dataBaseName());
        }
        db.close();

        if(!db.open())
        {
            return false;
        }

        if(!executeQuery(db, "create table tDatas("
                         "fStorage varchar(255) not null, "
                         "fProduct varchar(255) not null, "
                         "fDate date not null, "
                         "fSold numeric(15,2) not null, "
                         "fRest numeric(15,2) not null, "
                         "primary key(fStorage, fProduct, fDate));"))
        {
            return false;
        }

        if(!executeQuery(db, "create table tAnalogs("
                         "fMain varchar(255) not null, "
                         "fAnalog varchar(255) not null);"))
        {
            return false;
        }
        executeQuery(db, "create index iAnalogsAnalog on tAnalogs"
                         "(fAnalog);");
        executeQuery(db, "create index iAnalogsMain on tAnalogs"
                         "(fMain);");
        db.close();
    }
    QSqlDatabase::removeDatabase(connName);

    return true;
}

bool MySqlDataBase::remove()
{
    if(!isExist())
    {
        return true;
    }
    bool exist = true;

    if(!db_.open())
    {
        exist = false;
    }
    if(exist)
    {
        QSqlQuery query(db_);
        QString req("drop database %1;");
        if(!query.exec(req.arg(info_.dataBaseName())))
        {
            exist = false;
        }
    }
    return exist;
}

bool MySqlDataBase::insertWithManyValues(const QString &beginOfRequest,
                                         const QString &valuesRequet,
                                         const QList<QVariantList> &data)
{
    if(data.count() == 0
            || beginOfRequest.isEmpty()
            || valuesRequet.isEmpty())
    {
        return false;
    }
    QSqlQuery query(db_);

    const int bufferSuzeMax = 2000;
    int counter = 0;
    const int dataSize = data.at(0).count();
    beginTransaction();

    while(counter < dataSize)
    {
        int delta = 0;
        if(counter + bufferSuzeMax <= dataSize)
        {
            delta = bufferSuzeMax;
        }
        else
        {
            delta = dataSize - counter;
        }

        QString request(beginOfRequest);

        for(int j = counter; j < counter + delta ; j++)
        {
            request += valuesRequet;
        }

        request = request.left(request.length() - 1);
        request += ";";
        query.prepare(request);

        for(int k = counter; k < counter + delta; k++)
        {
            foreach (const QVariantList &list, data)
            {
                query.addBindValue(list.at(k));
            }
        }

        if(!query.exec())
        {
            qInfo() << query.lastError().text();
            rollbackTransaction();
            return false;
        }

        counter += delta;
    }
    commitTransaction();

    return true;
}

bool MySqlDataBase::isExist()
{
    QString conn = "isExistConn";
    bool exist = true;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", conn);
        db.setHostName(info_.hostName());
        db.setUserName(info_.userName());
        db.setPassword(info_.password());
        db.setDatabaseName(info_.dataBaseName());

        if(!db.open())
        {
            exist = false;
        }
        if(exist)
        {
            QSqlQuery query(db);
            if(!query.exec("show databases;"))
            {
                exist = false;
            }
            if(exist)
            {
                while(query.next())
                {
                    const QSqlRecord rec = query.record();
                    for(int i = 0; i < rec.count(); i++)
                    {
                        if(rec.value(i).toString() == info_.dataBaseName())
                        {
                            exist = true;
                            break;
                        }
                    }
                    if(exist)
                    {
                        break;
                    }
                }
            }
        }
    }
    QSqlDatabase::removeDatabase(conn);

    return exist;
}

bool MySqlDataBase::createTempTableForSalesHistoryStreamReader()
{
    QSqlQuery query(db_);
    db_.transaction();

    if(!query.exec("create temporary table tTempItems("
                   "fStorage varchar(255) not null, "
                   "fProduct varchar(255) not null, "
                   "fMainAn varchar(255));"))
    {
        qInfo()  << "cannot create temp table tTempItems";
        db_.rollback();
        return false;
    }

    if(!query.exec("create temporary table tTempOrder("
                   "fOrder integer auto_increment primary key, "
                   "fStorage varchar(255) not null, "
                   "fProduct varchar(255) not null, "
                   "fMainAn varchar(255), "
                   "unique(fStorage, fProduct));"))
    {
        qInfo()  << "cannot create temp table tTempOrder";
        qInfo() << query.lastError().text();
        db_.rollback();
        return false;
    }

    db_.commit();

    return true;
}

bool MySqlDataBase::createTempTableForAnalogsReader()
{
    QSqlQuery query(db_);
    db_.transaction();
    if(!query.exec("create temporary table if not exists tTempIdMain("
                   "fMain varchar(255), "
                   "fId varchar(255));"))
    {
        db_.rollback();
        return false;
    }

    if(!query.exec("create temporary table if not exists tTempIds("
                   "fId varchar(255) not null);"))
    {
        db_.rollback();
        return false;
    }
    db_.commit();
    return true;
}

bool MySqlDataBase::insertValuesToTDatas(const QList<SaleHistoryDay> &days)
{
    if(days.count() == 0)
    {
        return true;
    }
    QVariantList storageList;
    QVariantList productList;
    QVariantList dateList;
    QVariantList soldList;
    QVariantList restList;

    for(int j = 0; j <  days.count() ; j++)
    {
        const SaleHistoryDay day = days.at(j);
        if(day.isValid())
        {
            storageList << day.item().storage();
            productList << day.item().product();
            dateList << day.date().toString("yyyy.MM.dd");
            soldList << day.sold();
            restList << day.rest();
        }
    }

    QString beginOfRequest("insert into tDatas(fStorage, fProduct, fDate, fSold, fRest) "
                           "values");

    QString valuesRequet("(?, ?, ?, ?, ?),");

    QList<QVariantList> data;
    data << storageList
         << productList
         << dateList
         << soldList
         << restList;

    return insertWithManyValues(beginOfRequest, valuesRequet, data);
}

bool MySqlDataBase::insertValuesToTAnalogs(const AnalogsTable &table)
{
    QString beginOfRequest("insert into tAnalogs (fMain, fAnalog)  "
                           "values");

    QString valuesRequet("(?, ?),");

    QVariantList mains;
    QVariantList anVarLits;

    const QList<Analogs> anList = table.toList();
    if(anList.count() == 0)
    {
        return true;
    }

    foreach(const Analogs &an, anList)
    {
        const ID mainAn = an.mainAnalog();
        const QList<ID> analogs = an.toList();
        foreach (const ID &id, analogs)
        {
            mains << mainAn;
            anVarLits << id;
        }
    }

    QList<QVariantList> data;

    data << mains
         << anVarLits;

    return insertWithManyValues(beginOfRequest, valuesRequet, data);
}
