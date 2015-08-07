#include "SqliteSelByProductDataSelector.h"

SqliteSelByProductDataSelector::SqliteSelByProductDataSelector(const QString &dbName)
    :DataBase(dbName),
      caseValue_()
{
    connect();
}

SqliteSelByProductDataSelector::~SqliteSelByProductDataSelector()
{
}

const QSqlQuery SqliteSelByProductDataSelector::get()
{
    if(caseValue_.isEmpty())
    {
        return QSqlQuery();
    }
    else
    {
        QSqlQuery query=getAssociatedQuery();
        query.prepare("select t_items.f_product, "
                             "t_items.f_storage, "
                             "t_datas.f_date, "
                             "t_datas.f_sold, "
                             "t_datas.f_rest "
                      "from t_items left join t_datas "
                      "on t_items.f_id = t_datas.f_item "
                      "where t_items.f_product = :product;"
                      );
        query.bindValue(":product", caseValue_);
        if(!query.exec())
        {
            return QSqlQuery();
        }

        return query;
    }
}

void SqliteSelByProductDataSelector::setCaseValue(const QString &value)
{
    caseValue_ = value;
}
