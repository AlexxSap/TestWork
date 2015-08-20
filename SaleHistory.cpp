#include "SaleHistory.h"

bool operator != (const SaleHistory &left, const SaleHistory &right)
{
    return (left.item() != right.item() || left.days() != right.days());
}

bool operator == (const SaleHistory &left, const SaleHistory &right)
{
    return (left.item() == right.item() && left.days() == right.days());
}

SaleHistory::SaleHistory()
    :item_(),
      days_()
{

}

SaleHistory::SaleHistory(const Item &item)
    :item_(item),
      days_()
{

}

SaleHistory::SaleHistory(const SaleHistory &other)
    :item_(other.item()),
      days_()
{
    const QList<SaleHistoryDay> days = other.days();
    foreach (const SaleHistoryDay &day, days)
    {
        days_.insert(day.date(), Day(day.sold(), day.rest()));
    }
}

Item SaleHistory::item() const
{
    return item_;
}

Date SaleHistory::from() const
{
    if(!days_.isEmpty())
    {
        return days_.firstKey();
    }
    else
    {
        return Date();
    }
}

Date SaleHistory::to() const
{
    if(!days_.isEmpty())
    {
        return days_.lastKey();
    }
    else
    {
        return Date();
    }
}

void SaleHistory::addDay(const SaleHistoryDay &day)
{
    if(item() == day.item())
    {
        days_.insert(day.date(), Day(day.sold(), day.rest()));
    }
}

SaleHistory &SaleHistory::operator <<(const SaleHistoryDay &day)
{
    addDay(day);
    return *this;
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
        return SaleHistoryDay();
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
    SaleHistoryDay temp(item_, tempDate, day.sold(), day.rest());
    return temp;
}

QList<SaleHistoryDay> SaleHistory::days() const
{
    QList<SaleHistoryDay> temp;

    QMap<Date, Day>::const_iterator i = days_.begin();
    for( ;i != days_.end(); i++)
    {
        const Day d = i.value();
        SaleHistoryDay tempDay(item_, i.key(), d.sold(), d.rest());
        temp.append(tempDay);
    }
    return temp;
}

bool SaleHistory::isValid() const
{
    return (item().isValid() && !days_.isEmpty());
}

QString SaleHistory::toString() const
{
    QString str = item().toString();

    const QList<SaleHistoryDay> shDays = days();
    foreach (const SaleHistoryDay day, shDays)
    {
        str += day.toStringWoItem();
    }
    return str;
}

SaleHistory SaleHistory::normalaze(const Date &nFrom, const Date &nTo) const
{
    SaleHistory history(item_);

    Date factFrom = from();
    Date factTo = to();
    Date normFrom = nFrom;
    Date normTo = nTo;

    if(factFrom == Date() && factTo == Date() && normFrom == Date() && normTo == Date())
    {
        return history;
    }

    if(nFrom == Date())
    {
        normFrom = factFrom;
    }

    if(nTo == Date())
    {
        normTo = factTo;
    }

    if(days_.isEmpty())
    {
        history.addDay(SaleHistoryDay(item_, normFrom, 0.0, 0.0));
        factFrom = normFrom;
        factTo = normFrom;
    }

    if(normFrom < factFrom)
    {
        for(Date d = normFrom; d < factFrom; d = d.addDays(1))
        {
            history.addDay(SaleHistoryDay(item_, d, 0.0, 0.0));
        }
    }

    if(normTo > factTo)
    {
        Amount lastRest = 0.0;
        if(!days_.isEmpty())
        {
            lastRest = days_.last().rest();
        }
        for(Date d = factTo.addDays(1); d <= normTo; d = d.addDays(1))
        {
            history.addDay(SaleHistoryDay(item_, d, 0.0, lastRest));
        }
    }

    Amount prevRest = 0.0;
    if(!days_.isEmpty())
    {
        prevRest = days_.value(factFrom).rest();
    }
    for(Date d = factFrom; d <= factTo; d = d.addDays(1))
    {
        if(days_.contains(d))
        {
            prevRest = days_.value(d).rest();
            history.addDay(SaleHistoryDay(item_, d, days_.value(d).sold(), prevRest));
        }
        else
        {
            history.addDay(SaleHistoryDay(item_, d, 0.0, prevRest));
        }
    }

    return history;
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
