#include "SalesHistoryStreamReader.h"

SalesHistoryStreamReader::SalesHistoryStreamReader(const QList<Item> &items,
                                                   const QString &dbName)
    :items_(items),
      db_(dbName),
      query_(),
      currentIndex_(0),
      from_(),
      to_()
{

}

bool SalesHistoryStreamReader::open(const Date &from, const Date &to)
{
    if(items_.isEmpty())
    {
        return false;
    }
    from_ = from;
    to_ = to;

    query_ = db_.getAssociatedQuery();

    if(from_ == Date() && to_ == Date())
    {
        QString product;
        QString storage;
        foreach (const Item &item, items_)
        {
            product += "'" + item.product() + "', ";
            storage += "'" + item.storage() + "', ";
        }
        product = product.left(product.length()-2);
        storage = storage.left(storage.length()-2);

        query_.prepare("select f_storage, "
                      "f_product, "
                      "f_date, "
                      "f_sold, "
                      "f_rest "
                      "from t_datas "
                      "where f_storage in (" + storage + ") "
                      "and f_product in (" + product + ") "
                      "order by f_storage, f_product, f_date;");
    }
    else
    {
        QString product;
        QString storage;
        foreach (const Item &item, items_)
        {
            product += "'" + item.product() + "', ";
            storage += "'" + item.storage() + "', ";
        }
        product = product.left(product.length()-2);
        storage = storage.left(storage.length()-2);

        query_.prepare("select f_storage, "
                      "f_product, "
                      "f_date, "
                      "f_sold, "
                      "f_rest "
                      "from t_datas "
                      "where f_date >= :fromDate and f_date <= :toDate "
                      "and f_storage in (" + storage + ") "
                      "and f_product in (" + product + ") "
                      "order by f_storage, f_product, f_date;");

        query_.bindValue(":fromDate", from_.toString("yyyy.MM.dd"));
        query_.bindValue(":toDate", to_.toString("yyyy.MM.dd"));
    }
    if(!query_.exec())
    {
        return false;
    }

    if(!query_.lastError().type() == QSqlError::NoError)
    {
        return false;
    }
    return true;
}

bool SalesHistoryStreamReader::next()
{
    currentIndex_++;
    if(currentIndex_ < items_.count())
    {
        return true;
    }
    return false;
}

SaleHistory SalesHistoryStreamReader::current()
{
    const Item item = items_.at(currentIndex_);
    SaleHistory history(item);

    while(query_.next())
    {
        const QString storage = query_.value(0).toString();
        const QString product = query_.value(1).toString();
        const Item temp(storage, product);
        if(item != temp)
        {
            query_.previous();
            return history;
        }
        const QDate date = query_.value(2).toDate();
        const double sold = query_.value(3).toDouble();
        const double rest = query_.value(4).toDouble();
        history.addDay(SaleHistoryDay(item, date, sold, rest));
    }
    return history;
}
