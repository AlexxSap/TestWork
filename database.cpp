#include "DataBase.h"

#include "SqliteDataBase.h"
#include "MySqlDataBase.h"

DataBase::DataBase(const DataBaseInfo &info,
                   const QString &connName)
    : QObject(0),
      info_(info),
      db_(),
      connectionName_(connName)
{

}

DataBase::DataBase(const DataBase &other)
    : QObject(0),
      info_(other.info()),
      db_(),
      connectionName_(other.connectionName())
{

}

DataBase::DataBase()
    : QObject(0),
      info_(),
      db_(),
      connectionName_()
{

}

DataBase::~DataBase()
{

}

bool DataBase::executeQuery(QSqlDatabase &db, const QString &request)
{
    QSqlQuery query(db);
    bool res = query.exec(request);
    if(!res)
    {
        qInfo() << query.lastError().text();
        return false;
    }
    return res;
}

bool DataBase::connect()
{
    if(!isExist())
    {
        if(!createEmptyDB())
        {
            return false;
        }
    }
    return db_.open();
}

bool DataBase::disconnect()
{
    const QString conName = db_.connectionName();
    bool res = false;
    if(db_.isOpen())
    {
        db_.close();
        res = true;
    }
    db_ = QSqlDatabase();
    db_.removeDatabase(conName);
    return res;
}

bool DataBase::isConnected()
{
    return db_.isOpen();
}

QHash<int, Item> DataBase::itemsHashTable()
{
    QSqlQuery query(db_);
    query.setForwardOnly(true);
    query.prepare("select fItem, fStorage, fProduct from tItems;");
    if(!query.exec())
    {
        qWarning() << query.lastError();
        return QHash<int, Item>();
    }

    QHash<int, Item> items;
    while(query.next())
    {
        const int id = query.value(0).toInt();
        const ID storage = query.value(1).toString();
        const ID product = query.value(2).toString();
        items.insert(id, Item(storage, product));
    }
    return items;
}

QSqlQuery DataBase::associatedQuery() const
{
    return QSqlQuery(db_);
}

const DataBaseInfo DataBase::info() const
{
    return info_;
}


void DataBase::beginTransaction()
{
    db_.transaction();
}

void DataBase::rollbackTransaction()
{
    db_.rollback();
}

void DataBase::commitTransaction()
{
    db_.commit();
}

const QString DataBase::connectionName() const
{
    return connectionName_;
}

void DataBase::dropTempTableForAnalogsReader()
{
    QSqlQuery query(db_);
    db_.transaction();
    query.exec("drop table if exists tTempIds;");
    query.exec("drop table if exists tTempIdMain;");
    db_.commit();
}

void DataBase::dropTempTableForSalesHistoryStreamReader()
{
    QSqlQuery query(db_);
    query.exec("drop table if exists tTempItems;");
    query.exec("drop table if exists tTempOrder;");
}

void DataBase::dropTempTableItemsForSalesHistoryStreamReader()
{
    QSqlQuery query(db_);
    query.exec("drop table if exists tTempItems;");
}

QSqlQuery DataBase::queryForAnalogsReader(const bool &forward)
{
    QSqlQuery query(db_);
    query.setForwardOnly(forward);
    if(query.prepare("select tAnalogs.fMain, tAnalogs.fAnalog "
                     "from tTempIdMain left outer join tAnalogs "
                     "on tAnalogs.fMain = tTempIdMain.fMain "
                     "where tTempIdMain.fMain is not null "
                     "order by tAnalogs.fMain;"))
    {
        return query;
    }
    return QSqlQuery();
}

bool DataBase::remDataBase(const DataBaseInfo &info)
{
    QPointer<DataBase> db;
    switch (DataBaseInfo::dataBaseType())
    {
    case DataBaseInfo::SQLITE:
    {
        db = new SqliteDataBase(info);
        break;
    }
    case DataBaseInfo::MYSQL:
    {
        db = new MySqlDataBase(info);
        break;
    }
    default:
        break;
    }

    const bool res = db->remove();
    db->disconnect();
    delete db;
    return res;
}

QPointer<DataBase> DataBase::getDataBase(const DataBaseInfo &info,
                                         const QString &connName)
{
    switch (DataBaseInfo::dataBaseType())
    {
    case DataBaseInfo::SQLITE:
    {
        QPointer<DataBase> db = new SqliteDataBase(info, connName);
        return db;
    }
    case DataBaseInfo::MYSQL:
    {
        QPointer<DataBase> db = new MySqlDataBase(info, connName);
        return db;
    }
    default:
        break;
    }

    return QPointer<DataBase>();
}
