#include "SaleHistoryParser.h"


SaleHistoryParser::SaleHistoryParser()
    :pattern_("(^[^;]+|\"[^\"]+\")%1"                                  //наименование товара
              "([^;]+|\"[^\"]+\")%1"                                  //наименование склада
              "([0-9]{4}\.(0[1-9]|1[012])\.(0[1-9]|1[0-9]|2[0-9]|3[01]))%1"   //дата
              "(\\d+(\.\\d{0,})?)%1"                                          //продали
              "(\\d+(\.\\d{0,})?)"),                                     //конечный остаток
      splitter_(";"),
      dateFormat_("yyyy.MM.dd"),
      isValid_(true),
      rx_()
{
    rx_ = QRegExp(pattern_.arg(splitter_));
    stdSplitter_ = ";";
}

void SaleHistoryParser::setSplitter(const QString &splitter)
{
    splitter_ = splitter;
    stdSplitter_ = splitter.toStdString();
    rx_ = QRegExp(pattern_.arg(splitter_));
}

void SaleHistoryParser::setDateFormat(const QString &dateFormat)
{
    dateFormat_ = dateFormat;
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
        list.append(day);
        if(!isValid_)
        {
            return QList<SaleHistoryDay>();
        }
    }

    return list;
}

SaleHistoryDay SaleHistoryParser::parseString(const QString &raw)
{
/*
    всё                                 1888

    только конструктор SaleHistoryDay   1124    -----
    и разделение                        1265
    и Item                              1312
    и Date                              1823    -----
    и 2хAmount                          1888

*/

    const QStringList strList = raw.split(splitter_);
    if(strList.count() != 5)
    {
        isValid_ = false;
        return SaleHistoryDay();
    }

        const Item item(strList.at(0), strList.at(1));
//        const Date date = QDate::fromString(strList.at(2), dateFormat_);
        const Date date = strToDate(strList.at(2));
//        const Amount sold = strList.at(3).toDouble();
//        const Amount rest = strList.at(4).toDouble();

    return SaleHistoryDay(Item(raw, "1"),
                          Date(2015,8,8),
                          1.0,
                          1.0);

//    return SaleHistoryDay(item,
//                          date,
//                          sold,
//                          rest);


//    if(!rx_.exactMatch(raw))
//    {
//        isValid_ = false;
//        return SaleHistoryDay();
//    }

//    const int count = rx_.captureCount();
//    if(count < 8)
//    {
//        isValid_ = false;
//        return SaleHistoryDay();
//    }
//    const ID product = rx_.cap(1);
//    const ID storage = rx_.cap(2);
//    const Date date = QDate::fromString(rx_.cap(3), dateFormat_);
//    const Amount sold = rx_.cap(6).toDouble();
//    const Amount rest = rx_.cap(8).toDouble();
//    const SaleHistoryDay day(Item(storage, product), date, sold, rest);

//    return day;
}

StdVector SaleHistoryParser::parseStdString(std::string raw)
{
    std::vector<std::string> result;

    size_t pos = 0;
    int i = 0;
    while ((pos = raw.find(stdSplitter_)) != std::string::npos)
    {
        result.push_back(raw.substr(0, pos));
        i++;
        raw.erase(0, pos + stdSplitter_.length());
    }
    result.push_back(raw);
    return result;
}

Date SaleHistoryParser::strToDate(const QString &str)
{
    QStringList lst = str.split(".");
    return QDate(lst.at(0).toInt(), lst.at(1).toInt(), lst.at(2).toInt());
}
