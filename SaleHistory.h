#ifndef SALEHISTORY_H
#define SALEHISTORY_H

#include <QMultiMap>
#include <QMetaType>
#include <QDebug>

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
    QMultiMap<Date, Day> days_;

private:
    void normalazeByAnalogs();
    void remDay(const Date &date);

public:
    SaleHistory();
    SaleHistory(const Item &item);
    SaleHistory(const SaleHistory & other);

    Item item() const;

    Date from() const;
    Date to() const;

    void addDay(const SaleHistoryDay &day);

    SaleHistory& operator << (const SaleHistoryDay &day);

    SaleHistoryDay day(const Date &date) const;
    QList<SaleHistoryDay> days() const;

    bool isValid() const;

    QString toString() const;

    SaleHistory normalaze(const Date &nFrom, const Date &nTo);
};

Q_DECLARE_METATYPE(SaleHistory)

bool operator != (const SaleHistory &left, const SaleHistory &right);
bool operator == (const SaleHistory &left, const SaleHistory &right);

inline QDebug operator<<(QDebug debug, const SaleHistory &history)
{
   debug << history.toString();
   return debug;
}

#endif // SALEHISTORY_H
