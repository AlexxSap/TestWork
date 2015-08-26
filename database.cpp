#include "DataBase.h"

DataBase::DataBase(const QString &dbName)
    :dbName_(dbName)
{
    if(!QFile::exists(dbName))
    {
        createEmptyDB();
    }
    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName(dbName_);
    connect();
}

DataBase::~DataBase()
{
    disconnect();
}

bool DataBase::createEmptyDB()
{
    const QString connName("createEmptyDB");
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connName);
        db.setDatabaseName(dbName_);
        if(!db.open())
        {
            return false;
        }
        db.close();

        if(!db.open())
        {
            return false;
        }
        setPragmaParameters(db);

        if(!executeQuery(db, "create table t_datas("
                         "f_storage text not null, "
                         "f_product text not null, "
                         "f_date real not null, "
                         "f_sold real not null, "
                         "f_rest real not null, "
                         "primary key(f_storage, f_product, f_date));"))
        {
            return false;
        }

        if(!executeQuery(db, "create table t_analogs("
                         "f_main text not null, "
                         "f_analog text not null);"))
        {
            return false;
        }
        executeQuery(db, "create index i_analogs_analog on t_analogs"
                         "(f_analog);");
        executeQuery(db, "create index i_analogs_main on t_analogs"
                         "(f_main);");


        db.close();
    }
    QSqlDatabase::removeDatabase(connName);
    return true;
}

void DataBase::setPragmaParameters(QSqlDatabase &db)
{
    QSqlQuery query(db);
    db.transaction();
    query.exec("PRAGMA temp_store = MEMORY;");
    query.exec("PRAGMA synchronous = OFF;");
    query.exec("PRAGMA count_changes = OFF;");
    query.exec("PRAGMA journal_mode = WAL;");
    query.exec("PRAGMA foreign_keys = ON;");
    //    query.exec("PRAGMA cache_size = -40;");
    db.commit();
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

bool DataBase::createTempTableForWrite()
{
    QSqlQuery query(db_);
    return query.exec("create temp table if not exists t_temp_datas("
                      "f_storage text not null, "
                      "f_product text not null, "
                      "f_date real not null, "
                      "f_sold real not null, "
                      "f_rest real not null, "
                      "primary key(f_storage, f_product, f_date));");
}

bool DataBase::dropTempTableForWrite()
{
    QSqlQuery query(db_);
    return query.exec("drop table if exists t_temp_datas;");
}
