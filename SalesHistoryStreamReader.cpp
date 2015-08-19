#include "SalesHistoryStreamReader.h"

SalesHistoryStreamReader::SalesHistoryStreamReader(const QList<Item> &items,
                                                   const QString &dbName)
    :items_(items),
      db_(dbName),
      query_(),
      from_(),
      to_(),
      select_()
{

}

bool SalesHistoryStreamReader::createTempItemsTable()
{
    QSqlQuery query = db_.getAssociatedQuery();
    db_.beginTransaction();
    if(!query.exec("create temp table t_temp_items("
                   "f_storage text not null, "
                   "f_product text not null, "
                   "unique(f_storage, f_product));"))
    {
        qInfo()  << "cannot create temp table";
        db_.rollbackTransaction();
        return false;
    }
    db_.commitTransaction();

    query.clear();

    QVariantList storageList;
    QVariantList productList;

    foreach (const Item &item, items_)
    {
        storageList << item.storage();
        productList << item.product();
    }

    query.prepare("insert into t_temp_items(f_storage, f_product) "
                  "values (?, ?);");
    query.addBindValue(storageList);
    query.addBindValue(productList);
    db_.beginTransaction();
    if(!query.execBatch())
    {
        db_.rollbackTransaction();
        qInfo() << query.lastError().text();
        qInfo() << query.lastQuery();
        deleteTempItemsTable();
        return false;
    }

    db_.commitTransaction();

    return true;
}

void SalesHistoryStreamReader::deleteTempItemsTable()
{
    QSqlQuery query = db_.getAssociatedQuery();
    db_.beginTransaction();
    query.exec("drop table if exists t_temp_items;");
    db_.commitTransaction();
}

bool SalesHistoryStreamReader::open(const Date &from, const Date &to)
{
    from_ = from;
    to_ = to;

    if(!createTempItemsTable())
    {
        return false;
    }

    if(from_ == Date() && to_ == Date())
    {
        select_ = QString("select t_datas.f_storage, "
                          "t_datas.f_product, "
                          "t_datas.f_date, "
                          "t_datas.f_sold, "
                          "t_datas.f_rest "
                          "from t_datas "
                          "inner join t_temp_items "
                          "on t_datas.f_storage = t_temp_items.f_storage "
                          "and t_datas.f_product = t_temp_items.f_product "
                          "order by t_datas.f_storage, t_datas.f_product, t_datas.f_date;");
    }
    else
    {
        select_ = QString("select t_datas.f_storage, "
                          "t_datas.f_product, "
                          "t_datas.f_date, "
                          "t_datas.f_sold, "
                          "t_datas.f_rest "
                          "from t_datas "
                          "inner join t_temp_items "
                          "on t_datas.f_storage = t_temp_items.f_storage "
                          "and t_datas.f_product = t_temp_items.f_product "
                          "where t_datas.f_date >= '%1' and t_datas.f_date <= '%2' "
                          "order by t_datas.f_storage, t_datas.f_product, t_datas.f_date;");
        select_ = select_.arg(from_.toString("yyyy.MM.dd"))
                .arg(to_.toString("yyyy.MM.dd"));
    }
    query_ = db_.getAssociatedQuery();

    if(!query_.exec(select_))
    {
        qWarning() << query_.lastError().text();
        qWarning() << query_.lastQuery();
        qWarning() << query_.boundValues();

        return false;
    }

    if(!query_.lastError().type() == QSqlError::NoError)
    {
        return false;
    }

    if(!query_.next())
    {
        qWarning() << query_.lastQuery();
        qWarning() << "fail query_.next()";
        return false;
    }
    return true;
}

bool SalesHistoryStreamReader::next()
{
    if(!query_.next())
    {
        deleteTempItemsTable();
        return false;
    }
    else
    {
        return true;
    }
}

SaleHistory SalesHistoryStreamReader::current()
{
    const Item item(query_.value(0).toString(), query_.value(1).toString());
    SaleHistory history(item);
    do
    {
        const Item tempItemp(query_.value(0).toString(), query_.value(1).toString());
        if(item != tempItemp)
        {
            query_.previous();
            return history;
        }

        const QDate date = query_.value(2).toDate();
        const double sold = query_.value(3).toDouble();
        const double rest = query_.value(4).toDouble();
        history.addDay(SaleHistoryDay(history.item(), date, sold, rest));
    } while(query_.next());
    return history;
}
