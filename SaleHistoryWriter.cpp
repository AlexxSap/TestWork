#include "SaleHistoryWriter.h"

SaleHistoryWriter::SaleHistoryWriter(const QString &dbName)
    :db_(dbName),
      bufferSize_(100000),
      itemTable_(),
      isFromFile_(false),
      maxId_(0)
{

}

bool SaleHistoryWriter::write(const QList<SaleHistoryDay> &days)
{
    if(!isFromFile_)
    {
        if(!fillItemHashTable())
        {
            return false;
        }
    }
    //    QVariantList storageList;
    //    QVariantList productList;
    QVariantList itemIdList;
    QVariantList dateList;
    QVariantList soldList;
    QVariantList restList;

    QSqlQuery query = db_.getAssociatedQuery();
    //    query.prepare("insert into t_datas(f_storage, f_product, f_date, f_sold, f_rest) "
    //                  "values(?, ?, ?, ?, ?);");
    query.prepare("insert into t_datas(f_item, f_date, f_sold, f_rest) "
                  "values(?, ?, ?, ?);");
    int i = 0;
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

        //        storageList.clear();
        //        productList.clear();
        itemIdList.clear();
        dateList.clear();
        soldList.clear();
        restList.clear();

        for(int j = i; j < i + delta ; j++)
        {
            const SaleHistoryDay day = days.at(j);
            if(day.isValid())
            {
                //                storageList << day.item().storage();
                //                productList << day.item().product();
                const int id = getItemId(Item(day.item().storage(), day.item().product()));
                if(id == -1)
                {
                    return false;
                }
                itemIdList << id;
                dateList << day.date().toString("yyyy.MM.dd");
                soldList << day.sold();
                restList << day.rest();
            }
        }
        i += delta;

        //        query.addBindValue(storageList);
        //        query.addBindValue(productList);
        query.addBindValue(itemIdList);
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

bool SaleHistoryWriter::fillItemHashTable()
{
    QSqlQuery query = db_.getAssociatedQuery();
    query.setForwardOnly(true);
    if(!query.exec("select f_item, f_storage, f_product from t_items;"))
    {
        qWarning() << query.lastError();
        qWarning() << query.executedQuery();
        return false;
    }
    while(query.next())
    {
        const int itemId = query.value(0).toInt();
        if(itemId > maxId_)
        {
            maxId_ = itemId;
        }
        const ID storage = query.value(1).toString();
        const ID product = query.value(2).toString();
        itemTable_.insert(itemId, Item(storage, product));
    }
    return true;
}

int SaleHistoryWriter::getItemId(const Item &item)
{
    int id = itemTable_.key(item, -1) ;
    if(id == -1)
    {
        id = maxId_ + 1;
        itemTable_.insert(id, item);

        QSqlQuery query = db_.getAssociatedQuery();
        query.prepare("insert into t_items(f_item, f_storage, f_product) "
                      "values (?, ?, ?);");
        query.addBindValue(id);
        query.addBindValue(item.storage());
        query.addBindValue(item.product());
        db_.beginTransaction();
        if(!query.exec())
        {
            db_.rollbackTransaction();
            qWarning() << query.lastError();
            qWarning() << query.executedQuery();

            return -1;
        }
        maxId_++;
        db_.commitTransaction();
    }
    return id;
}

bool SaleHistoryWriter::importFromFile(const QString &fileName)
{
    if(!fillItemHashTable())
    {
        return false;
    }
    isFromFile_ = true;

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
