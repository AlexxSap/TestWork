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

SaleHistoryDay::SaleHistoryDay()
    :item_(),
      date_(),
      sold_(),
      rest_()
{

}

SaleHistoryDay::SaleHistoryDay(const SaleHistoryDay &other)
    :item_(other.item()),
      date_(other.date()),
      sold_(other.sold()),
      rest_(other.rest())
{

}

SaleHistoryDay::SaleHistoryDay(const Item &item,
                               const Date &date,
                               const Amount &sold,
                               const Amount &rest)
    :item_(item),
      date_(date),
      sold_(sold),
      rest_(rest)
{

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

bool SaleHistoryDay::isValid() const
{
    return (item().isValid()
            && date() != Date()
            && sold() >= 0.0
            && rest() >= 0.0);
}


