#include "SqliteSelByStorageAndProductDataSelector.h"

SqliteSelByStorageAndProductDataSelector::SqliteSelByStorageAndProductDataSelector(const QString &dbName)
    :DataBase(dbName),
      product_(),
      storage_()
{
    connect();
}

const QSqlQuery SqliteSelByStorageAndProductDataSelector::get()
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
                      "order by f_storage, f_product, f_date;");

        query.bindValue(":product", product_);
        query.bindValue(":storage", storage_);

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

void SqliteSelByStorageAndProductDataSelector::setCaseProduct(const QString &product)
{
    product_ = product;
}

void SqliteSelByStorageAndProductDataSelector::setCaseStorage(const QString &storage)
{
    storage_ = storage;
}
