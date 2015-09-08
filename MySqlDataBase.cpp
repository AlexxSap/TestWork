#include "MySqlDataBase.h"

MySqlDataBase::MySqlDataBase()
    : DataBase()
{

}


void MySqlDataBase::init(const QString &connName)
{
    db_ = QSqlDatabase::addDatabase("QMYSQL", connName);
    db_.setDatabaseName(info_.dataBaseName());
    db_.setUserName(info_.userName());
    db_.setPassword(info_.password());
}

MySqlDataBase::MySqlDataBase(const DataBaseInfo &info,
                             const QString &connName)
    :DataBase(info, connName)
{
    init(connName);
}

MySqlDataBase::MySqlDataBase(const DataBase &other)
    :DataBase(other)
{
    init(other.connectionName());
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

        if(!executeQuery(db, "create table tItems("
                         "fItem integer primary key, "
                         "fStorage varchar(255) not null, "
                         "fProduct varchar(255) not null);"))
        {
            return false;
        }

        executeQuery(db, "create index iItems on tItems"
                         "(fItem);");

        if(!executeQuery(db, "create table tDatas("
                         "fItem integer, "
                         "fDate date not null, "
                         "fSold numeric(15,2) not null, "
                         "fRest numeric(15,2) not null, "
                         "primary key(fItem, fDate), "
                         "foreign key(fItem) references tItems(fItem));"))
        {
            return false;
        }

        executeQuery(db, "create index iDatas on tDatas"
                         "(fItem);");

        if(!executeQuery(db, "create table tAnalogs("
                         "fMain varchar(255) not null, "
                         "fAnalog varchar(255) not null);"))
        {
            return false;
        }
        //        executeQuery(db, "create index iAnalogsAnalog on tAnalogs"
        //                         "(fAnalog);");
        //        executeQuery(db, "create index iAnalogsMain on tAnalogs"
        //                         "(fMain);");
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

bool MySqlDataBase::insertWithManyValues(const QString &tableDescr,
                                         const QList<QVariantList> &data)
{
    if(data.count() == 0
            || tableDescr.isEmpty())
    {
        return false;
    }
    QSqlQuery query(db_);

    const int bufferSizeMax = 2000;
    int counter = 0;
    const int dataSize = data.at(0).count();

    QString valuesRequet("(");
    for(int i = 0; i < data.count(); i++)
    {
        valuesRequet += "?, ";
    }
    valuesRequet = valuesRequet.left(valuesRequet.length()-2);
    valuesRequet += "),";

    beginTransaction();

    while(counter < dataSize)
    {
        int delta = 0;
        if(counter + bufferSizeMax <= dataSize)
        {
            delta = bufferSizeMax;
        }
        else
        {
            delta = dataSize - counter;
        }

        QString request("insert into " + tableDescr + " values");

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
            qInfo() << query.lastQuery();
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

    //temporary
    if(!query.exec("create temporary table tTempItems("
                   "fItem integer not null, "
                   "fMainAn varchar(255));"))
    {
        qInfo()  << "cannot create temp table tTempItems";
        db_.rollback();
        return false;
    }

    //temporary
    if(!query.exec("create temporary table tTempOrder("
                   "fOrder integer not null auto_increment primary key, "
                   "fItem integer not null, "
                   "fStorage varchar(255) not null, "
                   "fProduct varchar(255) not null, "
                   "fMainAn varchar(255));"))
    {
        qInfo()  << "cannot create temp table tTempOrder";
        qInfo() << query.lastError().text();
        db_.rollback();
        return false;
    }

    executeQuery(db_, "create index iTempOrder on tTempOrder"
                      "(fStorage, fProduct);");

    executeQuery(db_, "create unique index iTempOrder2 on tTempOrder"
                      "(fItem);");

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

QString MySqlDataBase::selectForSalesHistoryStreamReader(const QDate &from, const QDate &to)
{
    QString select("select tItems.fStorage, "
                   "tItems.fProduct, "
                   "tDatas.fDate, "
                   "tDatas.fSold, "
                   "tDatas.fRest "
                   "from tTempOrder "
                   "left outer join tItems "
                   "on tItems.fItem = tTempOrder.fItem "
                   "left outer join tDatas "
                   "on tTempOrder.fItem = tDatas.fItem "
                   "%1 "
                   "order by tTempOrder.fOrder "
                   "limit :limit offset :offset;");

    QString dateCase;
    if(from != QDate() && to != QDate())
    {
        dateCase = "where (tDatas.fDate >= '%1' and "
                   "tDatas.fDate <= '%2') "
                   "or tDatas.fDate is null";
        dateCase = dateCase.arg(from.toString("yyyy.MM.dd"))
                .arg(to.toString("yyyy.MM.dd"));
    }
    else if (from == QDate() && to != QDate())
    {
        dateCase = "where tDatas.fDate <= '%1' "
                   "or tDatas.fDate is null";
        dateCase = dateCase.arg(to.toString("yyyy.MM.dd"));
    }
    else if (to == QDate() && from != QDate())
    {
        dateCase = "where tDatas.fDate >= '%1' "
                   "or tDatas.fDate is null";
        dateCase = dateCase.arg(from.toString("yyyy.MM.dd"));
    }
    select = select.arg(dateCase);
    return select;
}

bool MySqlDataBase::insertToTItems(const QHash<int, Item> &newItems)
{
    const QList<int> keys = newItems.keys();

    QVariantList keyList;
    QVariantList storageList;
    QVariantList productList;

    foreach (const int &key, keys)
    {
        keyList << key;
        const Item item =  newItems.value(key);
        storageList << item.storage();
        productList << item.product();
    }

    QString tableDescr("tItems(fItem, fStorage, fProduct)");

    QList<QVariantList> data;
    data << keyList
         << storageList
         << productList;

    return insertWithManyValues(tableDescr, data);

}

bool MySqlDataBase::insertValuesToTDatas(const QList<SaleHistoryDay> &days,
                                         const QHash<int, Item> &items,
                                         const QHash<int, Item> &newItems)
{
    if(days.count() == 0)
    {
        return true;
    }
    if(!insertToTItems(newItems))
    {
        return false;
    }

    QVariantList idList;
    QVariantList dateList;
    QVariantList soldList;
    QVariantList restList;

    for(int j = 0; j <  days.count() ; j++)
    {
        const SaleHistoryDay day = days.at(j);
        if(day.isValid())
        {
            const int id = items.key(day.item());
            idList << id;
            dateList << day.date().toString("yyyy.MM.dd");
            soldList << day.sold();
            restList << day.rest();
        }
    }

    QString tableDescr("tDatas(fItem, fDate, fSold, fRest)");

    QList<QVariantList> data;
    data << idList
         << dateList
         << soldList
         << restList;

    return insertWithManyValues(tableDescr, data);
}

bool MySqlDataBase::insertValuesToTAnalogs(const AnalogsTable &table)
{
    QString tableDescr("tAnalogs (fMain, fAnalog)");

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

    return insertWithManyValues(tableDescr, data);
}
