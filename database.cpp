#include "DataBase.h"
#include "SqliteDataBase.h"

#include "DataBaseInfo.h"

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
    if(!QFile::exists(info_.dataBaseName()))
    {
        createEmptyDB();
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

QSqlQuery DataBase::getAssociatedQuery() const
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
    if(!query.exec("create temporary table if not exists t_temp_idmain("
                   "f_main text, "
                   "f_id text);"))
    {
        db_.rollback();
        return false;
    }

    if(!query.exec("create temporary table if not exists t_temp_ids("
                   "f_id text not null);"))
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
    query.exec("drop table if exists t_temp_ids;");
    query.exec("drop table if exists t_temp_idmain;");
    db_.commit();
}

bool DataBase::createTempTableForSalesHistoryStreamReader()
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
                   "f_order integer primary key asc autoincrement, "
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

void DataBase::dropTempTableForSalesHistoryStreamReader()
{
    QSqlQuery query(db_);
    db_.transaction();
    query.exec("drop table if exists t_temp_items;");
    query.exec("drop table if exists t_temp_order;");
    db_.commit();
}

QSqlQuery DataBase::queryForAnalogsReader(const bool &forward)
{
    QSqlQuery query(db_);
    query.setForwardOnly(forward);
    query.prepare("select t_analogs.f_main, t_analogs.f_analog "
                  "from t_temp_idmain left outer join t_analogs "
                  "on t_analogs.f_main = t_temp_idmain.f_main "
                  "where t_temp_idmain.f_main is not null "
                  "order by t_analogs.f_main;");
    return query;
}

QSqlQuery DataBase::queryForSalesHistoryStreamReader(const QDate &from,
                                                     const QDate &to,
                                                     const bool &forward)
{
    QSqlQuery query(db_);
    query.setForwardOnly(forward);

    QString select("select t_temp_order.f_storage, "
                   "t_temp_order.f_product, "
                   "t_datas.f_date, "
                   "t_datas.f_sold, "
                   "t_datas.f_rest "
                   "from t_temp_order "
                   "left outer join t_datas "
                   "on t_temp_order.f_storage = t_datas.f_storage "
                   "and t_temp_order.f_product = t_datas.f_product "
                   "%1"
                   "order by t_temp_order.f_order;");

    QString dateCase;
    if(from != QDate() && to != QDate())
    {
        dateCase = "where (t_datas.f_date >= '%1' and "
                   "t_datas.f_date <= '%2') "
                   "or t_datas.f_date is null ";
        dateCase = dateCase.arg(from.toString("yyyy.MM.dd"))
                .arg(to.toString("yyyy.MM.dd"));
    }
    else if (from == QDate() && to != QDate())
    {
        dateCase = "where t_datas.f_date <= '%1' "
                   "or t_datas.f_date is null ";
        dateCase = dateCase.arg(to.toString("yyyy.MM.dd"));
    }
    else if (to == QDate() && from != QDate())
    {
        dateCase = "where t_datas.f_date >= '%1' "
                   "or t_datas.f_date is null ";
        dateCase = dateCase.arg(from.toString("yyyy.MM.dd"));
    }
    select = select.arg(dateCase);

    query.prepare(select);
    return query;
}

QPointer<DataBase> DataBase::getDataBase(const DataBaseInfo &info,
                                         const QString &connName)
{

    switch (DataBaseInfo::dataBaseType()) {
    case DataBaseInfo::SQLITE:
    {
        QPointer<DataBase> db = new SqliteDataBase(info, connName);
        return db;
    }
    default:
        break;
    }

    return QPointer<DataBase>();
}
