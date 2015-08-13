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
    QSqlQuery query=getAssociatedQuery();
    const QString request("select f_storage, "
                          "f_product, "
                          "f_date, "
                          "f_sold, "
                          "f_rest "
                          "from t_datas;");

    if(!query.exec(request))
    {
        return QSqlQuery();
    }
    return query;
}
