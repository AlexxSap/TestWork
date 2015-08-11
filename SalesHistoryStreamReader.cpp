#include "SalesHistoryStreamReader.h"

SalesHistoryStreamReader::SalesHistoryStreamReader(const QList<Item> &items)
    :items_(items),
      query_(),
      dbName_(),
      currentIndex_(0),
      from_(),
      to_()
{

}

void SalesHistoryStreamReader::setDbName(const QString &dbName)
{
    dbName_ = dbName;
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

    if(from_ == Date() && to_ == Date())
    {
        SqliteSelByStorageAndProductDataSelector db(dbName_);
        db.setCaseProduct(product);
        db.setCaseStorage(storage);
        query_ = db.get();
    }
    else
    {
        SqliteSelByStorageAndProductAndDateDataSelector db(dbName_);
        db.setCaseDateInterval(from_, to_);
        db.setCaseProduct(product);
        db.setCaseStorage(storage);
        query_ = db.get();
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
        return queryForNextItem();
    }
    return false;
}

SaleHistory SalesHistoryStreamReader::current()
{
    SaleHistory history(items_.at(currentIndex_));
    query_.first();
    if(!query_.isValid())
    {
        history.addDay(from_, 0.0, 0.0);
    }
    else
    {
        do
        {
            QDate date = query_.value(2).toDate();
            double sold = query_.value(3).toDouble();
            double rest = query_.value(4).toDouble();
            history.addDay(date, sold, rest);

        } while(query_.next());
    }
    return history;
}
