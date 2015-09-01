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

    typedef QMap<Date, Day> SHData;

private:
    Item item_;
    SHData days_;

    QList<ID> analoglist_;
    QList<SHData> analogData_;

private:
    SHData normalazeData(const SHData &days,
                       const Date &nFrom,
                       const Date &nTo) const;
    void addDefaultData();

public:
    SaleHistory();
    explicit SaleHistory(const Item &item);
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

    void normalaze(const Date &nFrom,
                   const Date &nTo,
                   const ID &mainAnalog);

    bool operator != (const SaleHistory &other) const;
    bool operator == (const SaleHistory &other) const;
};

Q_DECLARE_METATYPE(SaleHistory)

inline QDebug operator<<(QDebug debug, const SaleHistory &history)
{
   debug << history.toString();
   return debug;
}

inline uint qHash(const SaleHistory &history, uint seed)
{
    return qHash(history.item(), seed)
            + qHash(history.from(), seed)
            + qHash(history.to(), seed);
}

#endif // SALEHISTORY_H
