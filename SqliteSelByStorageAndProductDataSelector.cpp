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

        query.prepare("select t_items.f_product, "
                      "t_items.f_storage, "
                      "t_datas.f_date, "
                      "t_datas.f_sold, "
                      "t_datas.f_rest "
                      "from t_items left join t_datas "
                      "on t_items.f_id = t_datas.f_item "
                      "where t_items.f_product = :product "
                      "and t_items.f_storage = :storage "
                      "order by t_items.f_storage, t_items.f_product, t_datas.f_date;");

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
