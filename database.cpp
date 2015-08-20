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

        //        if(!executeQuery(db, "create table t_datas("
        //                         "f_storage text not null, "
        //                         "f_product text not null, "
        //                         "f_date real not null, "
        //                         "f_sold real not null, "
        //                         "f_rest real not null, "
        //                         "primary key(f_storage, f_product, f_date));"))
        //        {
        //            return false;
        //        }
        //        if(!executeQuery(db, "create index i_datas_sp on t_datas("
        //                         "f_storage, f_product);"))
        //        {
        //            return false;
        //        }

        //        if(!executeQuery(db, "create index i_datas_ord on t_datas("
        //                         "f_storage, f_product, f_date);"))
        //        {
        //            return false;
        //        }




        if(!executeQuery(db, "create table t_items("
                         "f_item integer primary key asc, "
                         "f_storage text not null, "
                         "f_product text not null, "
                         "unique(f_storage, f_product));"))
        {
            return false;
        }
        if(!executeQuery(db, "create index i_items_sp on t_items"
                         "(f_storage, f_product);"))
        {
            return false;
        }


        if(!executeQuery(db, "create table t_datas("
                         "f_item integer, "
                         "f_date real not null, "
                         "f_sold real not null, "
                         "f_rest real not null, "
                         "primary key(f_item, f_date),"
                         "foreign key(f_item) references t_items(f_item));"))
        {
            return false;
        }
        if(!executeQuery(db, "create index i_datas_item on t_datas"
                         "(f_item);"))
        {
            return false;
        }

        setPragmaParameters(db);
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
