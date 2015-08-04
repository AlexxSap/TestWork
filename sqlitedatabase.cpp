#include "SqliteDataBase.h"

SqliteDataBase::SqliteDataBase(const QString &dbName):_dbName(dbName)
{
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(_dbName);
}

SqliteDataBase::~SqliteDataBase()
{
    disconnect();
}

bool SqliteDataBase::connect()
{
    return _db.open();
}

bool SqliteDataBase::disconnect()
{
    const QString conName=_db.connectionName();
    bool res=false;
    if(_db.isOpen())
    {
        _db.close();
        res= true;
    }
    _db = QSqlDatabase();
    _db.removeDatabase(conName);
    return res;
}

bool SqliteDataBase::isConnected()
{
    return _db.isOpen();
}

bool SqliteDataBase::write(const QString &request)
{
    QSqlQuery query;
    return query.exec(request);
}

QSqlQuery SqliteDataBase::read(const QString &request)
{
    QSqlQuery query;
    if(query.exec(request))
    {
        return query;
    }
    return QSqlQuery();
}

QSqlQueryModel* SqliteDataBase::model(const QString &request)
{
    Q_UNUSED(request)
    return new QSqlQueryModel();
}
