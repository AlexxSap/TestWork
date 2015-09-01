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
    return false;
}

bool MySqlDataBase::createTempTableForSalesHistoryStreamReader()
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
                   "f_order integer primary key asc auto_increment, "
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


