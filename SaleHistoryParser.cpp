#include "SaleHistoryParser.h"

SaleHistoryParser::SaleHistoryParser()
    : splitter_(";"),
      dateFormat_("yyyy.MM.dd"),
      isValid_(false)
{

}

bool SaleHistoryParser::isValid() const
{
    return isValid_;
}

QList<SaleHistoryDay> SaleHistoryParser::parse(const QStringList &rawData)
{
    QList<SaleHistoryDay> list;

    foreach (const QString &buf, rawData)
    {
        const SaleHistoryDay day = parseString(buf);
        if(!isValid_)
        {
            return QList<SaleHistoryDay>();
        }
        list.append(day);
    }
    return list;
}

SaleHistoryDay SaleHistoryParser::parseString(const QString &raw)
{
    const QStringList strList = raw.split(splitter_);
    if(strList.count() != 5)
    {
        isValid_ = false;
        return SaleHistoryDay();
    }

    const Item item(strList.at(0), strList.at(1));
    const Date date = strToDate(strList.at(2));
    const Amount sold = strList.at(3).toDouble();
    const Amount rest = strList.at(4).toDouble();

    isValid_ = true;
    return SaleHistoryDay(item, date, sold, rest);
}

Date SaleHistoryParser::strToDate(const QString &str)
{
    QStringList lst = str.split(".");
    return QDate(lst.at(0).toInt(), lst.at(1).toInt(), lst.at(2).toInt());
}
