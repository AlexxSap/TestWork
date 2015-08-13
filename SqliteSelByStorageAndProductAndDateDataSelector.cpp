#include "SqliteSelByStorageAndProductAndDateDataSelector.h"

SqliteSelByStorageAndProductAndDateDataSelector::SqliteSelByStorageAndProductAndDateDataSelector(const QString &dbName)
    :DataBase(dbName),
      product_(),
      storage_(),
      fromDate_(),
      toDate_()
{
    connect();
}

const QSqlQuery SqliteSelByStorageAndProductAndDateDataSelector::get()
{
    if(product_.isEmpty() || storage_.isEmpty())
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
                      "where f_storage = :storage "
                      "and f_product = :product "
                      "and f_date between :fromDate and :toDate "
                      "order by f_storage, f_product, f_date;");

        query.bindValue(":product", product_);
        query.bindValue(":storage", storage_);
        query.bindValue(":fromDate", fromDate_.toString("yyyy.MM.dd"));
        query.bindValue(":toDate", toDate_.toString("yyyy.MM.dd"));

        if(!query.exec())
        {
            return QSqlQuery();
        }

        while(query.next())
        {
            //почему-то без этого все данные теряются, кроме первой строки
        }

        return query;
    }
}

void SqliteSelByStorageAndProductAndDateDataSelector::setCaseProduct(const QString &product)
{
    product_ = product;
}

void SqliteSelByStorageAndProductAndDateDataSelector::setCaseStorage(const QString &storage)
{
    storage_ = storage;
}

void SqliteSelByStorageAndProductAndDateDataSelector::setCaseDateInterval(const QDate &fromDate, const QDate &toDate)
{
    fromDate_ = fromDate;
    toDate_ = toDate;
}
