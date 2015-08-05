#include "SqliteAllDataDataSelector.h"

SqliteAllDataDataSelector::SqliteAllDataDataSelector(const QString &dbName)
    :DataBase(dbName)

{
    connect();
}

SqliteAllDataDataSelector::~SqliteAllDataDataSelector()
{

}

const QSqlQuery SqliteAllDataDataSelector::get()
{
    QSqlQuery query(getDB());
    const QString request("select t_items.f_product, "
                          "t_items.f_storage, "
                          "t_datas.f_date, "
                          "t_datas.f_sold, "
                          "t_datas.f_rest "
                          "from t_items left join t_datas "
                          "on t_items.f_id = t_datas.f_item;");

    if(!query.exec(request))
    {
        return QSqlQuery();
    }

    return query;
}
