#include "SqliteSelByDateDataSelector.h"

SqliteSelByDateDataSelector::SqliteSelByDateDataSelector(const QString &dbName)
    :DataBase(dbName),
      caseValue_()
{
    connect();
}

SqliteSelByDateDataSelector::~SqliteSelByDateDataSelector()
{
}

const QSqlQuery SqliteSelByDateDataSelector::get()
{
    if(caseValue_.isEmpty())
    {
        return QSqlQuery();
    }
    else
    {
        QSqlQuery query(getDB());
        query.prepare("select t_items.f_product, "
                      "t_items.f_storage, "
                      "t_datas.f_date, "
                      "t_datas.f_sold, "
                      "t_datas.f_rest "
                      "from t_items left join t_datas "
                      "on t_items.f_id = t_datas.f_item "
                      "where t_datas.f_date = :date;");
        query.bindValue(":date", caseValue_);
        if(!query.exec())
        {
            return QSqlQuery();
        }

        return query;
    }
}

void SqliteSelByDateDataSelector::setCaseValue(const QString &value)
{
    caseValue_ = value;
}
