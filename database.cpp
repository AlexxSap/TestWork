#include "DataBase.h"
#include "SqliteDataBase.h"

DataBase::DataBase(const QString &dbName,
                   const QString &connName)
    :dbName_(dbName),
      db_(),
      type_()
{

    db_ = QSqlDatabase::addDatabase("QSQLITE", connName);
    db_.setDatabaseName(dbName_);
}

DataBase::DataBase(const DataBase &other)
    :dbName_(other.name()),
      db_(),
      type_()
{
    db_ = QSqlDatabase::addDatabase("QSQLITE", other.db_.connectionName());
    db_.setDatabaseName(dbName_);
}

DataBase::DataBase()
    :dbName_(),
      db_(),
      type_()
{

}

DataBase::~DataBase()
{

}


bool DataBase::executeQuery(QSqlDatabase &db, const QString &request)
{
    QSqlQuery query(db);
    db.transaction();
    bool res = query.exec(request);
    if(!res)
    {
        db.rollback();
        return false;
    }
    db.commit();
    return res;
}

bool DataBase::connect()
{
    if(!QFile::exists(dbName_))
    {
        createEmptyDB();
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

QSqlQuery DataBase::getAssociatedQuery() const
{
    return QSqlQuery(db_);
}

const QString DataBase::name() const
{
    return dbName_;
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

DataBase::Type DataBase::type()
{
    return type_;
}

bool DataBase::createTempTableForAnalogsReader()
{
    QSqlQuery query(db_);
    db_.transaction();
    if(!query.exec("create temporary table if not exists t_temp_idmain("
                   "f_main text, "
                   "f_id text);"))
    {
        db_.rollback();
        return false;
    }

    if(!query.exec("create temporary table if not exists t_temp_ids("
                   "f_id text not null);"))
    {
        db_.rollback();
        return false;
    }
    db_.commit();
    return true;
}

void DataBase::dropTempTableForAnalogsReader()
{
    QSqlQuery query(db_);
    db_.transaction();
    query.exec("drop table if exists t_temp_ids;");
    query.exec("drop table if exists t_temp_idmain;");
    db_.commit();
}

bool DataBase::createTempTableForSalesHistoryStreamReader()
{
    QSqlQuery query(db_);
    db_.transaction();

    if(!query.exec("create temporary table t_temp_items("
                   "f_storage text not null, "
                   "f_product text not null, "
                   "f_main_an text);"))
    {
        qInfo()  << "cannot create temp table t_temp_items";
        db_.rollback();
        return false;
    }

    if(!query.exec("create temporary table t_temp_order("
                   "f_order integer primary key asc autoincrement, "
                   "f_storage text not null, "
                   "f_product text not null, "
                   "f_main_an text, "
                   "unique(f_storage, f_product));"))
    {
        qInfo()  << "cannot create temp table t_temp_order";
        db_.rollback();
        return false;
    }

    db_.commit();

    return true;
}

void DataBase::dropTempTableForSalesHistoryStreamReader()
{
    QSqlQuery query(db_);
    db_.transaction();
    query.exec("drop table if exists t_temp_items;");
    query.exec("drop table if exists t_temp_order;");
    db_.commit();
}

QPointer<DataBase> getDataBase(const QString &dbName,
                      const DataBase::Type &type,
                      const QString &connName)
{
    switch (type) {
    case DataBase::SQLITE:
    {
        QPointer<DataBase> db = new SqliteDataBase(dbName, connName);
        return db;
    }


    default:
        break;
    }
}
