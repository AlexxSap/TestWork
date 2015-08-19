#include "SaleHistoryWriter.h"

SaleHistoryWriter::SaleHistoryWriter(const QString &dbName)
    :db_(dbName),
      bufferSize_(100000)
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
    //затраты памяти при разных bufferSize_
    // при 1 млн - 530 мб
    // при 100к - 342 мб
    // при 10к - 322 мб
    while(i < days.count())
    {
        int delta;
        if(i + bufferSize_ >= days.count())
        {
            delta = days.count() - i;
        }
        else
        {
            delta = bufferSize_;
        }

        storageList.clear();
        productList.clear();
        dateList.clear();
        soldList.clear();
        restList.clear();

        for(int j = i; j < i + delta ; j++)
        {
            const SaleHistoryDay day = days.at(j);
            if(day.isValid())
            {
            storageList << day.item().storage();
            productList << day.item().product();
            dateList << day.date().toString("yyyy.MM.dd");
            soldList << day.sold();
            restList << day.rest();
            }

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
            qInfo() << query.lastError().text();
            qInfo() << query.lastQuery();
            return false;
        }
        db_.commitTransaction();
    }

    //    QSqlQuery query = db_.getAssociatedQuery();
    //    return query.exec("analyze t_datas;");

    return true;
}

bool SaleHistoryWriter::writeBuffer(const QStringList &list)
{
    SaleHistoryParser parser;
    if(list.isEmpty())
    {
        return true;
    }

    QList<SaleHistoryDay> days = parser.parse(list);

    if(!parser.isValid())
    {
        return false;
    }
    return write(days);
}

bool SaleHistoryWriter::importFromFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QTextStream ts(&file);
    ts.setCodec(QTextCodec::codecForName("Windows-1251"));

    int counter = 0;
    QStringList bufferList;

    while(!ts.atEnd())
    {
        counter++;
        const QString buffer = ts.readLine().trimmed();
        if(!buffer.isEmpty())
        {
            bufferList.append(buffer);
        }

        if(counter == bufferSize_ )
        {
            counter = 0;

            bool isWited = writeBuffer(bufferList);
            if(!isWited)
            {
                file.close();
                return false;
            }
            bufferList.clear();
        }
    }
    file.close();

    return writeBuffer(bufferList);
}

void SaleHistoryWriter::setBufferSize(const int size)
{
    bufferSize_ = size;
}
