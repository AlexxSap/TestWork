#include "SaleHistoryWriter.h"

SaleHistoryWriter::SaleHistoryWriter(const DataBaseInfo &info)
    : db_(DataBase::getDataBase(info)),
      bufferSize_(1000000),
      queryForWrite_()
{
    db_->connect();
    queryForWrite_ = db_->associatedQuery();
}

SaleHistoryWriter::~SaleHistoryWriter()
{
    db_->disconnect();
    delete db_;
}

bool SaleHistoryWriter::write(const QList<SaleHistoryDay> &days)
{
    if(days.count() == 0)
    {
        return true;
    }
    return db_->insertValuesToTDatas(days);
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
