#include "SalesHistoryStreamReader.h"

SalesHistoryStreamReader::SalesHistoryStreamReader(const QList<Item> &items,
                                                   const QString &dbName)
    :items_(items),
      db_(dbName),
      query_(),
      currentIndex_(0),
      from_(),
      to_(),
      limit_(1000000),
      offset_(0),
      select_()
{

}

bool SalesHistoryStreamReader::open(const Date &from, const Date &to)
{
    from_ = from;
    to_ = to;

    QString itemCase;
    if(!items_.isEmpty())
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

        itemCase = QString("f_storage in (" + storage + ") "
                                                                "and f_product in (" + product + ") ");
    }

    if(from_ == Date() && to_ == Date())
    {
        const QString where(items_.isEmpty()?"":"where ");
        select_ = QString("select f_storage, "
                          "f_product, "
                          "f_date, "
                          "f_sold, "
                          "f_rest "
                          "from t_datas "
                          + where + itemCase +
                          "order by f_storage, f_product, f_date "
                          "limit :limit offset :offset;");
    }
    else
    {
        const QString andCase(items_.isEmpty()?"":"and ");
        select_ = QString("select f_storage, "
                          "f_product, "
                          "f_date, "
                          "f_sold, "
                          "f_rest "
                          "from t_datas "
                          "where f_date >= '%1' and f_date <= '%2' "
                          + andCase + itemCase +
                          "order by f_storage, f_product, f_date "
                          "limit :limit offset :offset;");
        select_ = select_.arg(from_.toString("yyyy.MM.dd"))
                         .arg(to_.toString("yyyy.MM.dd"));
    }
    query_ = db_.getAssociatedQuery();

    return nextQueryByOffset();
}


bool SalesHistoryStreamReader::nextQueryByOffset()
{
    query_.clear();
    query_.prepare(select_);
    query_.bindValue(":limit", limit_);
    query_.bindValue(":offset", offset_);

    offset_ += limit_;

    if(!query_.exec())
    {
        return false;
    }

    if(!query_.lastError().type() == QSqlError::NoError)
    {
        return false;
    }
    return query_.next();
}

bool SalesHistoryStreamReader::nextRecord()
{
    if(!query_.next())
    {
        return nextQueryByOffset();
    }
    return true;
}

bool SalesHistoryStreamReader::next()
{
    if(items_.isEmpty())
    {
        return nextRecord();
    }

    currentIndex_++;
    if(currentIndex_ < items_.count())
    {
        return nextRecord();
    }
    return false;
}

SaleHistory SalesHistoryStreamReader::current()
{
    SaleHistory history;
    if(!items_.isEmpty())
    {
        const Item item = items_.at(currentIndex_);
        history = SaleHistory(item);
    }

    do
    {
        const QString storage = query_.value(0).toString();
        const QString product = query_.value(1).toString();
        const Item temp(storage, product);
        if(!history.item().isValid())
        {
            history = SaleHistory(temp);
        }
        if(history.item() != temp)
        {
            query_.previous();
            return history;
        }
        const QDate date = query_.value(2).toDate();
        const double sold = query_.value(3).toDouble();
        const double rest = query_.value(4).toDouble();
        history.addDay(SaleHistoryDay(history.item(), date, sold, rest));
    } while(nextRecord());
    return history;
}
