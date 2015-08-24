#include "CsvFile.h"

bool CsvFile::write(const QList<SaleHistoryDay> &days, const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        return false;
    }
    const QString dateFormat = QString("yyyy.MM.dd");

    QTextStream out(&file);
    out.setCodec(QTextCodec::codecForName("Windows-1251"));

    foreach (const SaleHistoryDay &day, days)
    {
        if(!day.isValid())
        {
            continue;
        }
        QString temp("%1;%2;%3;%4;%5");
        temp = temp.arg(day.item().storage())
                .arg(day.item().product())
                .arg(day.date().toString(dateFormat))
                .arg(QString::number(day.sold(),'f',2))
                .arg(QString::number(day.rest(),'f',2));

          out << temp << "\n";
    }
    file.close();
    return true;
}

QList<SaleHistoryDay> CsvFile::read(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        return QList<SaleHistoryDay>();
    }

    file.reset();
    QTextStream ts(&file);
    ts.setCodec(QTextCodec::codecForName("Windows-1251"));

    QStringList list;

    while(!ts.atEnd())
    {
        const QString buffer = ts.readLine().trimmed();
        list.append(buffer);
    }

    SaleHistoryParser parser;
    const QList<SaleHistoryDay> result = parser.parse(list);
    const bool valid = parser.isValid();
    if(!valid)
    {
        file.close();
        return QList<SaleHistoryDay>();
    }

    file.close();
    return result;
}
