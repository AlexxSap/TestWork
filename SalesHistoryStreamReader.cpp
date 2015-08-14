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

    return queryForNextItem();
}

bool SalesHistoryStreamReader::queryForNextItem()
{
    ID product = items_.at(currentIndex_).product();
    ID storage = items_.at(currentIndex_).storage();

    query_ = db_.getAssociatedQuery();

    if(from_ == Date() && to_ == Date())
    {
        query_.prepare("select f_storage, "
                      "f_product, "
                      "f_date, "
                      "f_sold, "
                      "f_rest "
                      "from t_datas "
                      "where f_storage = :storage "
                      "and f_product = :product "
                      "order by f_storage, f_product, f_date;");

        query_.bindValue(":product", product);
        query_.bindValue(":storage", storage);
    }
    else
    {
        query_.prepare("select f_storage, "
                      "f_product, "
                      "f_date, "
                      "f_sold, "
                      "f_rest "
                      "from t_datas "
                      "where f_date >= :fromDate and f_date <= :toDate "
                      "and f_storage = :storage "
                      "and f_product = :product "
                      "order by f_storage, f_product, f_date;");

        query_.bindValue(":product", product);
        query_.bindValue(":storage", storage);
        query_.bindValue(":fromDate", from_.toString("yyyy.MM.dd"));
        query_.bindValue(":toDate", to_.toString("yyyy.MM.dd"));
    }
    query_.exec();

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
        return queryForNextItem();
    }
    return false;
}

SaleHistory SalesHistoryStreamReader::current()
{
    const Item item = items_.at(currentIndex_);
    SaleHistory history(item);
    query_.first();
    if(!query_.isValid())
    {
        return history;
    }
    else
    {
        do
        {
            QDate date = query_.value(2).toDate();
            double sold = query_.value(3).toDouble();
            double rest = query_.value(4).toDouble();
            history.addDay(SaleHistoryDay(item, date, sold, rest));

        } while(query_.next());
    }
    return history;
}
