#ifndef SALEHISTORYDAY_H
#define SALEHISTORYDAY_H

#include <QMetaType>
#include <QDebug>

#include "Item.h"

class SaleHistoryDay
{
private:
    Item item_;
    Date date_;
    Amount sold_;
    Amount rest_;

public:
    SaleHistoryDay();
    SaleHistoryDay(const SaleHistoryDay &other);
    SaleHistoryDay(const Item &item,
                   const Date &date,
                   const Amount &sold,
                   const Amount &rest);

    Item item() const;
    Amount sold() const;
    Amount rest() const;
    Date date() const;

    bool isValid() const;

    QString toString() const;
    QString toStringWoItem() const;

    bool operator != (const SaleHistoryDay &other) const;
    bool operator == (const SaleHistoryDay &other) const;
};

Q_DECLARE_METATYPE(SaleHistoryDay)

inline QDebug operator<<(QDebug debug, const SaleHistoryDay &day)
{
   debug << day.toString();
   return debug;
}

inline uint qHash(const SaleHistoryDay &day, uint seed)
{
    return qHash(day.item(), seed) + qHash(day.date(), seed);
}

#endif // SALEHISTORYDAY_H
