#include "SaleHistoryDay.h"

bool operator != (const SaleHistoryDay &left, const SaleHistoryDay &right)
{
    return (left.item() != right.item())
            || (left.date() != right.date())
            || (left.sold() != right.sold())
            || (left.rest() != right.rest());
}

bool operator == (const SaleHistoryDay &left, const SaleHistoryDay &right)
{
    return (left.item() == right.item())
            && (left.date() == right.date())
            && (left.sold() == right.sold())
            && (left.rest() == right.rest());
}

SaleHistoryDay::SaleHistoryDay(const Item &item, const Date &date)
    :item_(item),
      date_(date)
{

}

SaleHistoryDay::SaleHistoryDay(const SaleHistoryDay &other)
    :item_(other.item()),
      date_(other.date()),
      sold_(other.sold()),
      rest_(other.rest())
{

}

void SaleHistoryDay::setSold(const Amount &sold)
{
    sold_ = sold;
}

void SaleHistoryDay::setRest(const Amount &rest)
{
    rest_ = rest;
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

