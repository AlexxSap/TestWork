#include "SaleHistoryWriter.h"

SaleHistoryWriter::SaleHistoryWriter(const QString &dbName)
    :db_(dbName)
{

}

bool SaleHistoryWriter::write(const QList<SaleHistoryDay> &days)
{
    db_.beginTransaction();
    foreach (const SaleHistoryDay &day, days)
    {
        QSqlQuery query = db_.getAssociatedQuery();
        const ID storage = day.item().storage();
        const ID product = day.item().product();
        const Date date = day.date();
        const Amount sold = day.sold();
        const Amount rest = day.rest();
        query.prepare("insert into t_datas(f_storage, f_product, f_date, f_sold, f_rest) "
                      "values(:storage, :product, :date, :sold, :rest);");
        query.bindValue(":storage", storage);
        query.bindValue(":product", product);
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

    QSqlQuery query = db_.getAssociatedQuery();
    const bool res = query.exec("analyze t_datas;");

    return res;
}
