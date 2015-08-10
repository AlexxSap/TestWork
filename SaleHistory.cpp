#include "SaleHistory.h"

SaleHistory::SaleHistory(const Item &item)
    :item_(item)
{

}

SaleHistory::SaleHistory(const SaleHistory &other)
    :item_(other.item())
{
    QList<SaleHistoryDay> days = other.days();
    days_.clear();

    foreach (SaleHistoryDay temp, days)
    {
        addDay(temp.date(), temp.sold(), temp.rest());
    }
}

Item SaleHistory::item() const
{
    return item_;
}

Date SaleHistory::from() const
{
    return days_.firstKey();
}

Date SaleHistory::to() const
{
    return days_.lastKey();
}

void SaleHistory::addDay(const Date &date, const Amount &sold, const Amount &rest)
{
    days_.insert(date, Day(sold, rest));

}

SaleHistoryDay SaleHistory::day(const Date &date) const
{
    Date tempDate = date;
    Day day;
    if(days_.contains(date))
    {
        day = days_.value(date);
    }
    else if(date < days_.firstKey() || date > days_.lastKey())
    {
        day = Day();
    }
    else
    {
        QMap<Date, Day>::const_iterator i = days_.end() - 1;
        for( ;i != days_.begin(); i--)
        {
            if(i.key() < date)
            {
                tempDate = i.key();
                day = i.value();
                break;
            }
        }
    }
    SaleHistoryDay temp(item_, tempDate);
    temp.setSold(day.sold());
    temp.setRest(day.rest());
    return temp;
}

QList<SaleHistoryDay> SaleHistory::days() const
{
    QList<SaleHistoryDay> temp;

    QMap<Date, Day>::const_iterator i = days_.begin();
    for( ;i != days_.end(); i++)
    {
        SaleHistoryDay tempDay(item_, i.key());
        Day d = i.value();
        tempDay.setSold(d.sold());
        tempDay.setRest(d.rest());

        temp.append(tempDay);
    }
    return temp;
}

bool SaleHistory::isValid() const
{
    if(days_.isEmpty())
    {
        return false;
    }
    return true;
}

SaleHistory::Day::Day()
    :sold_(0.0),
      rest_(0.0)
{

}

SaleHistory::Day::Day(const Amount &sold, const Amount &rest)
    :sold_(sold),
      rest_(rest)
{

}

Amount SaleHistory::Day::sold() const
{
    return sold_;
}

Amount SaleHistory::Day::rest() const
{
    return rest_;
}
