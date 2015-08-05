#include "DataBase.h"

DataBase::DataBase(const QString &dbName)
    :dbName_(dbName)
{
    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName(dbName_);
    connect();
}

DataBase::~DataBase()
{
    disconnect();
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

const QSqlQuery DataBase::get()
{
    return QSqlQuery();
}

const QSqlDatabase DataBase::getDB() const
{
    return db_;
}
