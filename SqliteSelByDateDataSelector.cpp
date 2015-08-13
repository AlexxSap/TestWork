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
        QSqlQuery query=getAssociatedQuery();
        query.prepare("select f_storage, "
                      "f_product, "
                      "f_date, "
                      "f_sold, "
                      "f_rest "
                      "from t_datas "
                      "where f_date = :date;");
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
