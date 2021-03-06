#include "SaleHistoryDay.h"

bool SaleHistoryDay::operator != (const SaleHistoryDay &other) const
{
    return (item_ != other.item())
            || (date_ != other.date())
            || (sold_ != other.sold())
            || (rest_ != other.rest());
}

bool SaleHistoryDay::operator == (const SaleHistoryDay &other) const
{
    return (item_ == other.item())
            && (date_ == other.date())
            && (sold_ == other.sold())
            && (rest_ == other.rest());
}

SaleHistoryDay::SaleHistoryDay()
    :item_(),
      date_(),
      sold_(),
      rest_()
{

}

SaleHistoryDay::SaleHistoryDay(const SaleHistoryDay &other)
    :item_(other.item()),
      date_(other.date()),
      sold_(other.sold()),
      rest_(other.rest())
{

}

SaleHistoryDay::SaleHistoryDay(const Item &item,
                               const Date &date,
                               const Amount &sold,
                               const Amount &rest)
    :item_(item),
      date_(date),
      sold_(sold),
      rest_(rest)
{

}

Item SaleHistoryDay::item() const
{
    return item_;
}

Amount SaleHistoryDay::sold() const
{
    return sold_;
}

Amount SaleHistoryDay::rest() const
{
    return rest_;
}

Date SaleHistoryDay::date() const
{
    return date_;
}

bool SaleHistoryDay::isValid() const
{
    return (item().isValid()
            && date() != Date()
            && date().isValid()
            && sold() >= 0.0
            && rest() >= 0.0);
}

QString SaleHistoryDay::toString() const
{
    QString str("%1 %2");
    str = str.arg(item().toString())
            .arg(toStringWoItem());
    return str;
}

QString SaleHistoryDay::toStringWoItem() const
{
    QString str("[%1;%2;%3]");
    str = str.arg(date().toString("yyyy.MM.dd"))
            .arg(QString::number(sold(),'f',2))
            .arg(QString::number(rest(),'f',2));
    return str;
}


