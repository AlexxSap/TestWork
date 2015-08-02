#include "SqliteSelByDateDataSelector.h"

SqliteSelByDateDataSelector::SqliteSelByDateDataSelector(const QString &dbName)
    :_db(dbName),
      _caseValue()
{
    _db.connect();
}

SqliteSelByDateDataSelector::~SqliteSelByDateDataSelector()
{
    _db.disconnect();
}

QSqlQuery SqliteSelByDateDataSelector::get()
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
                              "where t_datas.f_date='"+_caseValue+"';");
        QSqlQuery query=_db.read(request);
        return query;
    }
}

void SqliteSelByDateDataSelector::setCaseValue(const QString &value)
{
    _caseValue=value;
}
