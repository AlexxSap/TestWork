#include "SqliteAllDataDataSelector.h"

SqliteAllDataDataSelector::SqliteAllDataDataSelector(const QString &dbName)
    :_db(dbName)
{
    _db.connect();
}

SqliteAllDataDataSelector::~SqliteAllDataDataSelector()
{
    _db.disconnect();
}

QSqlQuery SqliteAllDataDataSelector::get()
{
    const QString request("select t_products.f_name, "
                          "t_datas.f_date, "
                          "t_datas.f_sold, "
                          "t_datas.f_rest "
                          "from t_products left join t_datas "
                          "on t_products.f_id=t_datas.f_product;");
    QSqlQuery query=_db.read(request);
    return query;
}


