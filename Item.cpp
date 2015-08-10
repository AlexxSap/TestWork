#include "Item.h"

bool operator != (const Item &left, const Item &right)
{
    return left.product() != right.product() || left.storage() != right.storage();
}

bool operator == (const Item &left, const Item &right)
{
    return left.product() == right.product() && left.storage() == right.storage();
}

Item::Item(const ID &storage, const ID &product)
    : storage_(storage),
      product_(product)
{

}

Item::Item(const Item &item)
    : storage_(item.storage()),
      product_(item.product())
{

}

ID Item::storage() const
{
    return storage_;
}

ID Item::product() const
{
    return product_;
}

Item &Item::operator=(const Item &anotherItem)
{
    storage_ = anotherItem.storage();
    product_ = anotherItem.product();
    return *this;
}

