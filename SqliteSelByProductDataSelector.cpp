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
        query.prepare("select f_storage, "
                      "f_product, "
                      "f_date, "
                      "f_sold, "
                      "f_rest "
                      "from t_datas "
                      "where f_product = :product;"
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
