#ifndef SALEHISTORY_H
#define SALEHISTORY_H

#include <QMap>

#include "Item.h"
#include "SaleHistoryDay.h"

class SaleHistory
{
private:
    class Day
    {
    private:
        Amount sold_;
        Amount rest_;
    public:
        Day();
        Day(const Amount &sold, const Amount &rest);
        Amount sold() const;
        Amount rest() const;
    };

private:
    Item item_;
    QMap<Date, Day> days_;

private:
    SaleHistory();

public:
    SaleHistory(const Item &item);
    SaleHistory(const SaleHistory &other);

    Item item() const;

    Date from() const;
    Date to() const;

    ///notes почему бы не использовать один аргумент типа SaleHistoryDay?
    /// можно было бы сразу проверять, что добавляется SaleHistoryDay правильного товара
    /// и не нужно будет переписывать использование этого метода из-за того, в SaleHistoryDay появилось новое поле
    void addDay(const Date &date, const Amount &sold, const Amount &rest);
    SaleHistoryDay day(const Date &date) const;
    QList<SaleHistoryDay> days() const;

    bool isValid() const;
};

#endif // SALEHISTORY_H
