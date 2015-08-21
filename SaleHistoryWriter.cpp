#include "SaleHistoryWriter.h"

SaleHistoryWriter::SaleHistoryWriter(const QString &dbName)
    :db_(dbName),
      bufferSize_(100000),
      itemTable_(),
      deltaItemTable_(),
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

    QSqlQuery query = db_.getAssociatedQuery();
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

        QVariantList itemIdList;
        QVariantList dateList;
        QVariantList soldList;
        QVariantList restList;

        for(int j = i; j < i + delta ; j++)
        {
            const SaleHistoryDay day = days.at(j);
            if(day.isValid())
            {
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

    if(!isFromFile_)
    {
        if(!appendItemTable())
        {
            return false;
        }
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

    /// todo: замерить сколько тратиться на распарсивание
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
        maxId_++;
        itemTable_.insert(maxId_, item);
        deltaItemTable_.insert(maxId_, item);
        return maxId_;
    }
    return id;
}

bool SaleHistoryWriter::appendItemTable()
{
    QSqlQuery query = db_.getAssociatedQuery();
    query.prepare("insert into t_items(f_item, f_storage, f_product) "
                  "values (?, ?, ?);");

    QVariantList idList;
    QVariantList storageList;
    QVariantList productList;
    for(QHash<int, Item>::const_iterator id = deltaItemTable_.cbegin();
        id != deltaItemTable_.cend(); id ++)
    {
        idList << id.key();
        storageList << id.value().storage();
        productList << id.value().product();

    }

    query.addBindValue(idList);
    query.addBindValue(storageList);
    query.addBindValue(productList);

    db_.beginTransaction();
    if(!query.execBatch())
    {
        db_.rollbackTransaction();
        qWarning() << query.lastError();
        qWarning() << query.executedQuery();

        return false;
    }
    db_.commitTransaction();
    return true;
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

    const bool isWrited = writeBuffer(bufferList);
    if(!isWrited)
    {
        return false;
    }

    return appendItemTable();
}

void SaleHistoryWriter::setBufferSize(const int size)
{
    bufferSize_ = size;
}
