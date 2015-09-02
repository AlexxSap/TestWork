#include "DataBase.h"

#include "SqliteDataBase.h"
#include "MySqlDataBase.h"

DataBase::DataBase(const DataBaseInfo &info,
                   const QString &connName)
    : QObject(0),
      info_(info),
      db_(),
      connectionName_(connName)
{

}

DataBase::DataBase(const DataBase &other)
    : QObject(0),
      info_(other.info()),
      db_(),
      connectionName_(other.connectionName())
{

}

DataBase::DataBase()
    : QObject(0),
      info_(),
      db_(),
      connectionName_()
{

}

DataBase::~DataBase()
{

}

bool DataBase::executeQuery(QSqlDatabase &db, const QString &request)
{
    QSqlQuery query(db);
    //    db.transaction();
    bool res = query.exec(request);
    if(!res)
    {
        qInfo() << query.lastQuery();
        qInfo() << query.lastError().text();
        //        db.rollback();
        return false;
    }
    //    db.commit();
    return res;
}

bool DataBase::connect()
{
    if(!isExist())
    {
        if(!createEmptyDB())
        {
            return false;
        }
    }

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

QSqlQuery DataBase::associatedQuery() const
{
    return QSqlQuery(db_);
}

const DataBaseInfo DataBase::info() const
{
    return info_;
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

const QString DataBase::connectionName() const
{
    return connectionName_;
}

bool DataBase::createTempTableForAnalogsReader()
{
    QSqlQuery query(db_);
    db_.transaction();
    if(!query.exec("create temporary table if not exists tTempIdMain("
                   "fMain text, "
                   "fId text);"))
    {
        db_.rollback();
        return false;
    }

    if(!query.exec("create temporary table if not exists tTempIds("
                   "fId text not null);"))
    {
        db_.rollback();
        return false;
    }
    db_.commit();
    return true;
}

void DataBase::dropTempTableForAnalogsReader()
{
    QSqlQuery query(db_);
    db_.transaction();
    query.exec("drop table if exists tTempIds;");
    query.exec("drop table if exists tTempIdMain;");
    db_.commit();
}

bool DataBase::createTempTableForSalesHistoryStreamReader()
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
                   "fOrder integer primary key asc autoincrement, "
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

void DataBase::dropTempTableForSalesHistoryStreamReader()
{
    QSqlQuery query(db_);
    db_.transaction();
    query.exec("drop table if exists tTempItems;");
    query.exec("drop table if exists tTempOrder;");
    db_.commit();
}

QSqlQuery DataBase::queryForAnalogsReader(const bool &forward)
{
    QSqlQuery query(db_);
    query.setForwardOnly(forward);
    if(query.prepare("select tAnalogs.fMain, tAnalogs.fAnalog "
                     "from tTempIdMain left outer join tAnalogs "
                     "on tAnalogs.fMain = tTempIdMain.fMain "
                     "where tTempIdMain.fMain is not null "
                     "order by tAnalogs.fMain;"))
    {
        return query;
    }
    return QSqlQuery();
}

QSqlQuery DataBase::queryForSalesHistoryStreamReader(const QDate &from,
                                                     const QDate &to,
                                                     const bool &forward)
{
    QSqlQuery query(db_);
    query.setForwardOnly(forward);

    QString select("select tTempOrder.fStorage, "
                   "tTempOrder.fProduct, "
                   "tDatas.fDate, "
                   "tDatas.fSold, "
                   "tDatas.fRest "
                   "from tTempOrder "
                   "left outer join tDatas "
                   "on tTempOrder.fStorage = tDatas.fStorage "
                   "and tTempOrder.fProduct = tDatas.fProduct "
                   "%1"
                   "order by tTempOrder.fOrder;");

    QString dateCase;
    if(from != QDate() && to != QDate())
    {
        dateCase = "where (tDatas.fDate >= '%1' and "
                   "tDatas.fDate <= '%2') "
                   "or tDatas.fDate is null ";
        dateCase = dateCase.arg(from.toString("yyyy.MM.dd"))
                .arg(to.toString("yyyy.MM.dd"));
    }
    else if (from == QDate() && to != QDate())
    {
        dateCase = "where tDatas.fDate <= '%1' "
                   "or tDatas.fDate is null ";
        dateCase = dateCase.arg(to.toString("yyyy.MM.dd"));
    }
    else if (to == QDate() && from != QDate())
    {
        dateCase = "where tDatas.fDate >= '%1' "
                   "or tDatas.fDate is null ";
        dateCase = dateCase.arg(from.toString("yyyy.MM.dd"));
    }
    select = select.arg(dateCase);

    if(query.prepare(select))
    {
        return query;
    }
    return QSqlQuery();
}

bool DataBase::remDataBase(const DataBaseInfo &info)
{
    QPointer<DataBase> db;
    switch (DataBaseInfo::dataBaseType())
    {
    case DataBaseInfo::SQLITE:
    {
        db = new SqliteDataBase(info);
        break;
    }
    case DataBaseInfo::MYSQL:
    {
        db = new MySqlDataBase(info);
        break;
    }
    default:
        break;
    }

    const bool res = db->remove();
    db->disconnect();
    return res;
}

QPointer<DataBase> DataBase::getDataBase(const DataBaseInfo &info,
                                         const QString &connName)
{
    switch (DataBaseInfo::dataBaseType())
    {
    case DataBaseInfo::SQLITE:
    {
        QPointer<DataBase> db = new SqliteDataBase(info, connName);
        return db;
    }
    case DataBaseInfo::MYSQL:
    {
        QPointer<DataBase> db = new MySqlDataBase(info, connName);
        return db;
    }
    default:
        break;
    }

    return QPointer<DataBase>();
}
