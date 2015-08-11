#include "SaleHistoryParser.h"

SaleHistoryParser::SaleHistoryParser()
    :pattern_("(^[^;]+|\"[^\"]+\")%1"                                  //наименование товара
              "([^;]+|\"[^\"]+\")%1"                                  //наименование склада
              "([0-9]{4}\.(0[1-9]|1[012])\.(0[1-9]|1[0-9]|2[0-9]|3[01]))%1"   //дата
              "(\\d+(\.\\d{0,})?)%1"                                          //продали
              "(\\d+(\.\\d{0,})?)"),                                     //конечный остаток
    splitter_(";"),
    dateFormat_("yyyy.MM.dd"),
    isValid_(false)
{

}

void SaleHistoryParser::setSplitter(const QString &splitter)
{
    splitter_ = splitter;
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
    QString pattern = pattern_.arg(splitter_);
    const QRegExp rx(pattern);

    foreach (QString buf, rawData)
    {
        if(!rx.exactMatch(buf))
        {
            isValid_ = false;
            return QList<SaleHistoryDay>();
        }

        const ID product = rx.cap(1);
        const ID storage = rx.cap(2);
        const Date date = QDate::fromString(rx.cap(3), dateFormat_);
        Amount sold = rx.cap(6).toDouble();
        Amount rest = rx.cap(8).toDouble();

        SaleHistoryDay day(Item(storage, product), date);
        day.setSold(sold);
        day.setRest(rest);

        list.append(day);
    }

    isValid_ = true;
    return list;
}

