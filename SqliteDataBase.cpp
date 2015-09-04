#include "SqliteDataBase.h"

SqliteDataBase::SqliteDataBase()
    : DataBase()
{

}


void SqliteDataBase::init(const QString &connName)
{
    db_ = QSqlDatabase::addDatabase("QSQLITE", connName);
    db_.setDatabaseName(info_.dataBaseName());
}

SqliteDataBase::SqliteDataBase(const DataBaseInfo &info,
                               const QString &connName)
    :DataBase(info, connName)
{
    init(connName);
}

SqliteDataBase::SqliteDataBase(const DataBase &other)
    :DataBase(other)
{
    init(other.connectionName());
}

SqliteDataBase::~SqliteDataBase()
{

}

bool SqliteDataBase::createEmptyDB()
{
    const QString connName("createEmptyDBSqlite");
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connName);
        db.setDatabaseName(info_.dataBaseName());
        if(!db.open())
        {
            return false;
        }
        db.close();

        if(!db.open())
        {
            return false;
        }

        executeQuery(db,"PRAGMA temp_store = MEMORY;");
        executeQuery(db,"PRAGMA synchronous = OFF;");
        executeQuery(db,"PRAGMA count_changes = OFF;");
        executeQuery(db,"PRAGMA journal_mode = WAL;");
        executeQuery(db,"PRAGMA foreign_keys = ON;");

        if(!executeQuery(db, "create table tItems("
                         "fItem integer primary key asc, "
                         "fStorage text not null, "
                         "fProduct text not null, "
                         "unique(fStorage, fProduct));"))
        {
            return false;
        }

        if(!executeQuery(db, "create table tDatas("
                         "fItem integer, "
                         "fDate real not null, "
                         "fSold real not null, "
                         "fRest real not null, "
                         "primary key(fItem, fDate), "
                         "foreign key(fItem) references tItems(fItem));"))
        {
            return false;
        }

        if(!executeQuery(db, "create table tAnalogs("
                         "fMain text not null, "
                         "fAnalog text not null);"))
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

bool SqliteDataBase::remove()
{
    disconnect();
    if(isExist())
    {
        if(!QFile::remove(info_.dataBaseName()))
        {
            return false;
        }
    }
    return true;
}

bool SqliteDataBase::isExist()
{
    return QFile::exists(info_.dataBaseName());
}

bool SqliteDataBase::insertToTItems(const QHash<int, Item> &newItems)
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

    QSqlQuery query(db_);
    query.prepare("insert into tItems(fItem, fStorage, fProduct) "
                  "values(?, ?, ?);");

    query.addBindValue(keyList);
    query.addBindValue(storageList);
    query.addBindValue(productList);

    beginTransaction();
    if(!query.execBatch())
    {
        rollbackTransaction();
        qInfo() << query.lastError().text();
        return false;
    }
    commitTransaction();
    return true;

}

bool SqliteDataBase::insertValuesToTDatas(const QList<SaleHistoryDay> &days,
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

    QSqlQuery query(db_);
    query.prepare("insert into tDatas(fItem, fDate, fSold, fRest) "
                  "values(?, ?, ?, ?);");

    query.addBindValue(idList);
    query.addBindValue(dateList);
    query.addBindValue(soldList);
    query.addBindValue(restList);

    beginTransaction();
    if(!query.execBatch())
    {
        rollbackTransaction();
        qInfo() << query.lastError().text();
        return false;
    }
    commitTransaction();
    return true;
}

bool SqliteDataBase::insertValuesToTAnalogs(const AnalogsTable &table)
{
    const QList<Analogs> anList = table.toList();
    if(anList.isEmpty())
    {
        return true;
    }

    QSqlQuery query(db_);
    query.prepare("insert into tAnalogs (fMain, fAnalog) "
                  "values(?, ?);");

    QVariantList mains;
    QVariantList anVarLits;

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
    query.addBindValue(mains);
    query.addBindValue(anVarLits);

    beginTransaction();
    if(!query.execBatch())
    {
        qInfo() << query.lastError().text();
        rollbackTransaction();
        return false;
    }
    commitTransaction();
    return true;
}

bool SqliteDataBase::createTempTableForAnalogsReader()
{
    QSqlQuery query(db_);
    db_.transaction();
    if(!query.exec("create temporary table if not exists tTempIdMain("
                   "fMain text, "
                   "fId text);"))
    {
        db_.rollback();
        return false;
    }

    if(!query.exec("create temporary table if not exists tTempIds("
                   "fId text not null);"))
    {
        db_.rollback();
        return false;
    }
    db_.commit();
    return true;
}

bool SqliteDataBase::createTempTableForSalesHistoryStreamReader()
{
    QSqlQuery query(db_);
    db_.transaction();

    if(!query.exec("create temporary table tTempItems("
                   "fItem integer not null, "
                   "fMainAn text);"))
    {
        qInfo()  << "cannot create temp table tTempItems";
        db_.rollback();
        return false;
    }

    if(!query.exec("create temporary table tTempOrder("
                   "fOrder integer primary key asc autoincrement, "
                   "fItem integer not null, "
                   "fMainAn text);"))
    {
        qInfo()  << "cannot create temp table tTempOrder";
        db_.rollback();
        return false;
    }

    db_.commit();

    return true;
}

QString SqliteDataBase::selectForSalesHistoryStreamReader(const QDate &from, const QDate &to, const bool &forward)
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
                   "%1"
                   "order by tTempOrder.fOrder;");

    QString dateCase;
    if(from != QDate() && to != QDate())
    {
        dateCase = "where (tDatas.fDate >= '%1' and "
                   "tDatas.fDate <= '%2') "
                   "or tDatas.fDate is null ";
        dateCase = dateCase.arg(from.toString("yyyy.MM.dd"))
                .arg(to.toString("yyyy.MM.dd"));
    }
    else if (from == QDate() && to != QDate())
    {
        dateCase = "where tDatas.fDate <= '%1' "
                   "or tDatas.fDate is null ";
        dateCase = dateCase.arg(to.toString("yyyy.MM.dd"));
    }
    else if (to == QDate() && from != QDate())
    {
        dateCase = "where tDatas.fDate >= '%1' "
                   "or tDatas.fDate is null ";
        dateCase = dateCase.arg(from.toString("yyyy.MM.dd"));
    }
    select = select.arg(dateCase);

    return select;
}
