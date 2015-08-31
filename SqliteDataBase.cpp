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

