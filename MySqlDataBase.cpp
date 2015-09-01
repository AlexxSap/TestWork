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

    if(!query.exec("create temporary table tTempItems("
                   "fStorage text not null, "
                   "fProduct text not null, "
                   "fMainAn text);"))
    {
        qInfo()  << "cannot create temp table tTempItems";
        db_.rollback();
        return false;
    }

    if(!query.exec("create temporary table tTempOrder("
                   "fOrder integer primary key asc auto_increment, "
                   "fStorage text not null, "
                   "fProduct text not null, "
                   "fMainAn text, "
                   "unique(fStorage, fProduct));"))
    {
        qInfo()  << "cannot create temp table tTempOrder";
        db_.rollback();
        return false;
    }

    db_.commit();

    return true;
}


