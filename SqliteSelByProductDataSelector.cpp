#include "SqliteSelByProductDataSelector.h"

SqliteSelByProductDataSelector::SqliteSelByProductDataSelector(const QString &dbName)
    :_db(dbName),
      _caseValue()
{
    _db.connect();
}

SqliteSelByProductDataSelector::~SqliteSelByProductDataSelector()
{
    _db.disconnect();
}

QSqlQuery SqliteSelByProductDataSelector::get()
{
    if(_caseValue.isEmpty())
    {
        return QSqlQuery();
    }
    else
    {
        const QString request("select t_products.f_name, "
                              "t_datas.f_date, "
                              "t_datas.f_sold, "
                              "t_datas.f_rest "
                              "from t_products left join t_datas "
                              "on t_products.f_id=t_datas.f_product "
                              "where t_products.f_name='"+_caseValue+"';");
        QSqlQuery query=_db.read(request);
        return query;
    }
}

void SqliteSelByProductDataSelector::setCaseValue(const QString &value)
{
    ///notes все бинарные операторы выделяем пробелами с обоих сторон
    ///  _caseValue = value;
    _caseValue=value;
}

