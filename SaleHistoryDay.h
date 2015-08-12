#ifndef SALEHISTORYDAY_H
#define SALEHISTORYDAY_H

#include "Item.h"

class SaleHistoryDay
{
private:
    Item item_;
    Date date_;
    Amount sold_;
    Amount rest_;

private:
    SaleHistoryDay();

public:
    SaleHistoryDay(const Item &item, const Date &date);

    ///notes зачем нужен в явном виде указанный конструктор копирования?
    SaleHistoryDay(const SaleHistoryDay &other);

    ///notes сеттеры очень опасные штуки, особенно в public доступе. желательно максимально их избегать(для простых "плоских" структур это обычно получается без проблем)
    /// и все необходимые данные для класса подавать в конструкторе.
    /// конкретно в этом классе это бы дало возможность создавать только корректные дни продаж, в которых всегда точно заданы и продажи и остаток.
    /// день с продажами, но без остатка, может вызывать кучу проблем в дальнейшей разработке
    void setSold(const Amount &sold);
    void setRest(const Amount &rest);

    Item item() const;
    Amount sold() const;
    Amount rest() const;
    Date date() const;
};


bool operator != (const SaleHistoryDay &left, const SaleHistoryDay &right);
bool operator == (const SaleHistoryDay &left, const SaleHistoryDay &right);

#endif // SALEHISTORYDAY_H
