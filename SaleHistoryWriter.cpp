#include "SaleHistoryWriter.h"

SaleHistoryWriter::SaleHistoryWriter(const DataBaseInfo &info)
    : db_(DataBase::getDataBase(info)),
      bufferSize_(100000),
      queryForWrite_()
{
    db_->connect();
    queryForWrite_ = db_->associatedQuery();
//    queryForWrite_.prepare("insert into tDatas(fStorage, fProduct, fDate, fSold, fRest) "
//                           "values(?, ?, ?, ?, ?), (?, ?, ?, ?, ?);");
}

SaleHistoryWriter::~SaleHistoryWriter()
{
    db_->disconnect();
    delete db_;
}

bool SaleHistoryWriter::write(const QList<SaleHistoryDay> &days)
{
    int i = 0;
    if(days.count() == 0)
    {
        return true;
    }
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

        QString request("insert into tDatas(fStorage, fProduct, fDate, fSold, fRest) "
                        "values");
        for(int v = 0; v < delta; v++)
        {
            request += "(?, ?, ?, ?, ?),";
        }
        request = request.left(request.length() - 1);
        request += ";";

        queryForWrite_.prepare(request);

//        QVariantList storageList;
//        QVariantList productList;
//        QVariantList dateList;
//        QVariantList soldList;
//        QVariantList restList;

        for(int j = i; j < i + delta ; j++)
        {
            const SaleHistoryDay day = days.at(j);
            if(day.isValid())
            {
//                storageList << day.item().storage();
//                productList << day.item().product();
//                dateList << day.date().toString("yyyy.MM.dd");
//                soldList << day.sold();
//                restList << day.rest();
                queryForWrite_.addBindValue(day.item().storage());
                queryForWrite_.addBindValue(day.item().product());
                queryForWrite_.addBindValue(day.date().toString("yyyy.MM.dd"));
                queryForWrite_.addBindValue(day.sold());
                queryForWrite_.addBindValue(day.rest());

            }
        }
        i += delta;

        db_->beginTransaction();

        if(!queryForWrite_.exec())
        {
            db_->rollbackTransaction();
            qInfo() << queryForWrite_.executedQuery();
            qInfo() << queryForWrite_.lastError().text();
            qInfo() << queryForWrite_.lastQuery();
            return false;
        }
        db_->commitTransaction();
    }
    return true;
}

bool SaleHistoryWriter::checkFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QString splitter(";");
    QString pattern("(^[^;]+|\"[^\"]+\")%1"
                    "([^;]+|\"[^\"]+\")%1"
                    "([0-9]{4}\.(0[1-9]|1[012])\.(0[1-9]|1[0-9]|2[0-9]|3[01]))%1"
                    "(\\d+(\.\\d{0,})?)%1"
                    "(\\d+(\.\\d{0,})?)");
    QRegExp rx = QRegExp(pattern.arg(splitter));

    QTextStream ts(&file);
    ts.setCodec(QTextCodec::codecForName("Windows-1251"));

    while(!ts.atEnd())
    {
        const QString str = ts.readLine().trimmed();
        if(!rx.exactMatch(str))
        {
            file.close();
            return false;
        }
    }
    file.close();
    return true;
}

bool SaleHistoryWriter::importFromFile(const QString &fileName)
{
    if(!checkFile(fileName))
    {
        return false;
    }

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QTextStream ts(&file);
    ts.setCodec(QTextCodec::codecForName("Windows-1251"));

    int counter = 0;
    QList<SaleHistoryDay> bufferList;
    SaleHistoryParser parser;

    while(!ts.atEnd())
    {
        counter++;
        const QString buffer = ts.readLine().trimmed();

        if(!buffer.isEmpty())
        {
            const SaleHistoryDay day = parser.parseString(buffer);
            bufferList.append(day);
        }

        if(counter == bufferSize_ )
        {
            counter = 0;

            bool isWited = write(bufferList);
            if(!isWited)
            {
                file.close();
                return false;
            }
            bufferList.clear();
        }
    }
    file.close();
    if(!write(bufferList))
    {
        return false;
    }
    return true;
}

void SaleHistoryWriter::setBufferSize(const int &size)
{
    bufferSize_ = size;
}
