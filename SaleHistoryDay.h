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
    SaleHistoryDay(const SaleHistoryDay &shd);

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
