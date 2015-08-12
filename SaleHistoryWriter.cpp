#include "SaleHistoryWriter.h"

int SaleHistoryWriter::getItemId(const ID &product, const ID &storage)
{
    if(product.isEmpty() || storage.isEmpty())
    {
        return -1;
    }

    QSqlQuery query = db_.getAssociatedQuery();
    query.prepare("select f_id from t_items where f_product = :product and f_storage = :storage;");
    query.bindValue(":product", product);
    query.bindValue(":storage", storage);
    if(!query.exec())
    {
        return -1;
    }

    if(!query.first())
    {
        QSqlQuery insQuery = db_.getAssociatedQuery();
        insQuery.prepare("insert into t_items(f_product, f_storage) values(:product, :storage);");
        insQuery.bindValue(":product", product);
        insQuery.bindValue(":storage", storage);

        bool isOk = insQuery.exec();
        if(isOk)
        {
            isOk = query.exec();
        }
        if(isOk)
        {
            isOk = query.first();
        }

        if(!isOk)
        {
            return -1;
        }
    }

    bool isOk = false;
    int id = query.value(0).toInt(&isOk);
    if(!isOk)
    {
        return -1;
    }
    return id;
}

SaleHistoryWriter::SaleHistoryWriter(const QString &dbName)
    :db_(dbName)
{

}

bool SaleHistoryWriter::write(const QList<SaleHistoryDay> &days)
{
    db_.beginTransaction();
    foreach (SaleHistoryDay day, days)
    {
        ///notes const ID product = day.item().product();
        ID product = day.item().product();
        ID storage = day.item().storage();

        ///notes что за itemId? чем обосновано его введение в схему?
        int itemId = getItemId(product, storage);
        if(itemId < 0)
        {
            db_.rollbackTransaction();
            return false;
        }
        QSqlQuery query = db_.getAssociatedQuery();
        Date date = day.date();
        Amount sold = day.sold();
        Amount rest = day.rest();
        query.prepare("insert into t_datas(f_item, f_date, f_sold, f_rest) values(:id, :date, :sold, :rest);");
        query.bindValue(":id", itemId);
        query.bindValue(":date", date.toString("yyyy.MM.dd"));
        query.bindValue(":sold", sold);
        query.bindValue(":rest", rest);
        if(!query.exec())
        {
            db_.rollbackTransaction();
            return false;
        }
    }
    db_.commitTransaction();
    return true;
}
