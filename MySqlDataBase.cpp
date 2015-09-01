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
    const QString connName("createEmptyDBMySql");
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", connName);
        db.setHostName(info_.hostName());
        db.setUserName(info_.userName());
        db.setPassword(info_.password());

        if(!db.open())
        {
            return false;
        }
        else
        {
            QSqlQuery query(db);
            QString req("create database if not exists %1;");
            if(!query.prepare(req.arg(info_.dataBaseName())))
            {
                db.close();
                return false;
            }
            if(!query.exec())
            {
                db.close();
                return false;
            }
            db.setDatabaseName(info_.dataBaseName());
        }
        db.close();

        if(!db.open())
        {
            return false;
        }

        if(!executeQuery(db, "create table tDatas("
                         "fStorage varchar(255) not null, "
                         "fProduct varchar(255) not null, "
                         "fDate date not null, "
                         "fSold real not null, "
                         "fRest real not null, "
                         "primary key(fStorage, fProduct, fDate));"))
        {
            return false;
        }

        if(!executeQuery(db, "create table tAnalogs("
                         "fMain varchar(255) not null, "
                         "fAnalog varchar(255) not null);"))
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

bool MySqlDataBase::isExist()
{
    QString conn = "isExistConn";
    bool exist = true;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", conn);
        db.setHostName(info_.hostName());
        db.setUserName(info_.userName());
        db.setPassword(info_.password());
        db.setDatabaseName(info_.dataBaseName());

        if(!db.open())
        {
            exist = false;
        }
        if(exist)
        {
            QSqlQuery query(db);
            if(!query.exec("show databases;"))
            {
                exist = false;
            }
            if(exist)
            {
                while(query.next())
                {
                    const QSqlRecord rec = query.record();
                    for(int i = 0; i < rec.count(); i++)
                    {
                        if(rec.value(i).toString() == info_.dataBaseName())
                        {
                            exist = true;
                            break;
                        }
                    }
                    if(exist)
                    {
                        break;
                    }
                }
            }
        }
    }
    QSqlDatabase::removeDatabase(conn);

    return exist;
}

bool MySqlDataBase::createTempTableForSalesHistoryStreamReader()
{
    QSqlQuery query(db_);
    db_.transaction();

    if(!query.exec("create temporary table tTempItems("
                   "fStorage varchar(255) not null, "
                   "fProduct varchar(255) not null, "
                   "fMainAn varchar(255));"))
    {
        qInfo()  << "cannot create temp table tTempItems";
        db_.rollback();
        return false;
    }

    if(!query.exec("create temporary table tTempOrder("
                   "fOrder integer auto_increment primary key, "
                   "fStorage varchar(255) not null, "
                   "fProduct varchar(255) not null, "
                   "fMainAn varchar(255), "
                   "unique(fStorage, fProduct));"))
    {
        qInfo()  << "cannot create temp table tTempOrder";
        qInfo() << query.lastError().text();
        db_.rollback();
        return false;
    }

    db_.commit();

    return true;
}


