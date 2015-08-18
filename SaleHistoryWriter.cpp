#include "SaleHistoryWriter.h"

SaleHistoryWriter::SaleHistoryWriter(const QString &dbName)
    :db_(dbName)
{

}

bool SaleHistoryWriter::write(const QList<SaleHistoryDay> &days)
{
    QVariantList storageList;
    QVariantList productList;
    QVariantList dateList;
    QVariantList soldList;
    QVariantList restList;

    int i = 0;
    //затраты памяти при разных size
    // при 1 млн - 530 мб
    // при 100к - 342 мб
    // при 10к - 322 мб
    const int size = 100000;
    while(i < days.count())
    {
        int delta;
        if(i + size >= days.count())
        {
            delta = days.count() - i;
        }
        else
        {
            delta = size;
        }

        storageList.clear();
        productList.clear();
        dateList.clear();
        soldList.clear();
        restList.clear();

        for(int j = i; j < i + delta ; j++)
        {
            storageList << days.at(j).item().storage();
            productList << days.at(j).item().product();
            dateList << days.at(j).date().toString("yyyy.MM.dd");
            soldList << days.at(j).sold();
            restList << days.at(j).rest();
        }
        i += delta;

        QSqlQuery query = db_.getAssociatedQuery();
        query.prepare("insert into t_datas(f_storage, f_product, f_date, f_sold, f_rest) "
                      "values(?, ?, ?, ?, ?);");
        query.addBindValue(storageList);
        query.addBindValue(productList);
        query.addBindValue(dateList);
        query.addBindValue(soldList);
        query.addBindValue(restList);

        db_.beginTransaction();
        if(!query.execBatch())
        {
            db_.rollbackTransaction();
            return false;
        }
        db_.commitTransaction();
    }

    //    QSqlQuery query = db_.getAssociatedQuery();
    //    return query.exec("analyze t_datas;");

    return true;
}
