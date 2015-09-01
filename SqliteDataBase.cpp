#include "SqliteDataBase.h"

SqliteDataBase::SqliteDataBase()
    : DataBase()
{

}

SqliteDataBase::SqliteDataBase(const DataBaseInfo &info,
                               const QString &connName)
    :DataBase(info, connName)
{
    db_ = QSqlDatabase::addDatabase("QSQLITE", connName);
    db_.setDatabaseName(info.dataBaseName());
}

SqliteDataBase::SqliteDataBase(const DataBase &other)
    :DataBase(other)
{
    db_ = QSqlDatabase::addDatabase("QSQLITE",
                                    other.connectionName());
    db_.setDatabaseName(info_.dataBaseName());
}

SqliteDataBase::~SqliteDataBase()
{

}

bool SqliteDataBase::createEmptyDB()
{
    const QString connName("createEmptyDB");
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


        if(!executeQuery(db, "create table tDatas("
                         "fStorage text not null, "
                         "fProduct text not null, "
                         "fDate real not null, "
                         "fSold real not null, "
                         "fRest real not null, "
                         "primary key(fStorage, fProduct, fDate));"))
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

