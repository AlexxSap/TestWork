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

        query.prepare("select t_items.f_product, "
                      "t_items.f_storage, "
                      "t_datas.f_date, "
                      "t_datas.f_sold, "
                      "t_datas.f_rest "
                      "from t_items left join t_datas "
                      "on t_items.f_id = t_datas.f_item "
                      "where t_items.f_product = :product "
                      "and t_items.f_storage = :storage "
                      "and t_datas.f_date between :fromDate and :toDate "
                      "order by t_items.f_storage, t_items.f_product, t_datas.f_date;");

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
