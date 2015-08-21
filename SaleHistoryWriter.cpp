#include "SaleHistoryWriter.h"

SaleHistoryWriter::SaleHistoryWriter(const QString &dbName)
    :db_(dbName),
      bufferSize_(1000000),
      queryForWrite_()
{
    queryForWrite_ = db_.getAssociatedQuery();
    queryForWrite_.prepare("insert into t_datas(f_storage, f_product, f_date, f_sold, f_rest) "
                  "values(?, ?, ?, ?, ?);");
}

bool SaleHistoryWriter::write(const QList<SaleHistoryDay> &days)
{
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

        QVariantList storageList;
        QVariantList productList;
        QVariantList dateList;
        QVariantList soldList;
        QVariantList restList;

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

        queryForWrite_.addBindValue(storageList);
        queryForWrite_.addBindValue(productList);
        queryForWrite_.addBindValue(dateList);
        queryForWrite_.addBindValue(soldList);
        queryForWrite_.addBindValue(restList);

        db_.beginTransaction();
        if(!queryForWrite_.execBatch())
        {
            db_.rollbackTransaction();
            qInfo() << queryForWrite_.lastError().text();
            qInfo() << queryForWrite_.lastQuery();
            return false;
        }
        db_.commitTransaction();
    }

    return true;
}

bool SaleHistoryWriter::writeStd(const QList<StdVector> &days)
{
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

        QVariantList storageList;
        QVariantList productList;
        QVariantList dateList;
        QVariantList soldList;
        QVariantList restList;

        for(int j = i; j < i + delta ; j++)
        {
            const StdVector day = days.at(j);

                storageList << QString::fromStdString(day.at(0));
                productList << QString::fromStdString(day.at(1));
                dateList << QString::fromStdString(day.at(2));
                soldList << QString::fromStdString(day.at(3));
                restList << QString::fromStdString(day.at(4));

        }
        i += delta;

        queryForWrite_.addBindValue(storageList);
        queryForWrite_.addBindValue(productList);
        queryForWrite_.addBindValue(dateList);
        queryForWrite_.addBindValue(soldList);
        queryForWrite_.addBindValue(restList);

        db_.beginTransaction();
        if(!queryForWrite_.execBatch())
        {
            db_.rollbackTransaction();
            qInfo() << queryForWrite_.lastError().text();
            qInfo() << queryForWrite_.lastQuery();
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
    QList<SaleHistoryDay> bufferList;
//    QList<StdVector> bufferList;
    SaleHistoryParser parser;

    while(!ts.atEnd())
    {
        counter++;
        const QString buffer = ts.readLine().trimmed();

        if(!buffer.isEmpty())
        {
            const SaleHistoryDay day = parser.parseString(buffer);
//            StdVector day = parser.parseStdString(buffer.toStdString());
            bufferList.append(day);
        }

        if(counter == bufferSize_ )
        {
            counter = 0;

            bool isWited = write(bufferList);
//            bool isWited = writeStd(bufferList);
            if(!isWited)
            {
                file.close();
                return false;
            }
            bufferList.clear();
        }
    }
    file.close();
    return write(bufferList);
//    return writeStd(bufferList);
}

void SaleHistoryWriter::setBufferSize(const int size)
{
    bufferSize_ = size;
}
