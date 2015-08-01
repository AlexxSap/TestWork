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
    if(_db.isOpen())
    {
        const QString conName=_db.connectionName();
        _db.close();
        _db = QSqlDatabase();
        _db.removeDatabase(conName);
        return true;
    }
    return false;
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
