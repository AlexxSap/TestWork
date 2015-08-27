#include "SaleHistory.h"

bool operator != (const SaleHistory &left, const SaleHistory &right)
{
    return (left.item() != right.item() || left.days() != right.days());
}

bool operator == (const SaleHistory &left, const SaleHistory &right)
{
    return (left.item() == right.item() && left.days() == right.days());
}

SaleHistory::SHData SaleHistory::normalazeData(const SaleHistory::SHData &days,
                                               const Date &nFrom,
                                               const Date &nTo) const
{
    SaleHistory::SHData returnedData = days;
    Date factFrom = from();
    Date factTo = to();
    Date normFrom = nFrom;
    Date normTo = nTo;

    if(factFrom == Date() && factTo == Date()
            && normFrom == Date() && normTo == Date())
    {
        return returnedData;
    }

    if(nFrom == Date())
    {
        normFrom = factFrom;
    }

    if(nTo == Date())
    {
        normTo = factTo;
    }

    if(returnedData.isEmpty())
    {
        returnedData.insert(normFrom, Day(0.0, 0.0));
        factFrom = normFrom;
        factTo = normFrom;
    }
    // от даты начала до первой фактической
    if(normFrom < factFrom)
    {
        for(Date d = normFrom; d < factFrom; d = d.addDays(1))
        {
            returnedData.insert(d, Day(0.0, 0.0));
        }
    }
    //от последней фактической до конечной даты
    if(normTo > factTo)
    {
        Amount lastRest = returnedData.last().rest();
        for(Date d = factTo.addDays(1); d <= normTo; d = d.addDays(1))
        {
            returnedData.insert(d, Day(0.0, lastRest));
        }
    }

    //в промежутке от первой фактической до последней фактической
    Amount prevRest = returnedData.value(factFrom).rest();
    Amount prevSold = returnedData.value(factFrom).sold();

    for(Date d = factFrom; d <= factTo; d = d.addDays(1))
    {
        if(returnedData.contains(d))
        {
            prevRest = returnedData.value(d).rest();
            prevSold = returnedData.value(d).sold();
            returnedData.insert(d, Day(prevSold, prevRest));
        }
        else
        {
            returnedData.insert(d, Day(0.0, prevRest));
        }
    }
    return returnedData;
}

SaleHistory::SaleHistory()
    :item_(),
      days_(),
      analoglist_(),
      analogData_()
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
    const ID product = day.item().product();
    if(product != item_.product())
    {
        int index = 0;
        if(!analoglist_.contains(product))
        {
            analoglist_.append(product);
            QMap<Date, Day> map;
            map.insert(day.date(), Day(day.sold(), day.rest()));
            analogData_.append(map);
        }
        else
        {
            index = analoglist_.indexOf(product);
            if(index >= 0)
            {
                QMap<Date, Day> map = analogData_.takeAt(index);
                map.insert(day.date(), Day(day.sold(), day.rest()));
                analogData_.insert(index, map);
            }
        }
    }
    else
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

void SaleHistory::normalaze(const Date &nFrom, const Date &nTo, const ID &mainAnalog)
{
    days_ = normalazeData(days_, nFrom, nTo);

    if(analoglist_.isEmpty())
    {
        return;
    }

    if(!mainAnalog.isEmpty())
    {
        item_ = Item(item_.storage(), mainAnalog);
    }

    for(int i = 0; i < analogData_.count(); i++)
    {
        SHData temp =  analogData_.takeAt(i);
        temp = normalazeData(temp, nFrom, nTo);
        analogData_.insert(i, temp);
    }

    const Date fD = from();
    const Date fT = to();

    for(Date date = fD; date <= fT; date = date.addDays(1))
    {
        Day day = days_.take(date);
        Amount sold = day.sold();
        Amount rest = day.rest();
        foreach (const SHData data, analogData_)
        {
            const Day temp = data.value(date);
            sold += temp.sold();
            rest += temp.rest();
        }
        day = Day(sold, rest);
        days_.insert(date, day);
    }
    analogData_.clear();
    analoglist_.clear();
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

