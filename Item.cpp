#include "Item.h"

bool Item::operator != (const Item &other) const
{
    return product_ != other.product() || storage_ != other.storage();
}

bool Item::operator == (const Item &other) const
{
    return product_ == other.product() && storage_ == other.storage();
}

Item::Item()
    : storage_(),
      product_()
{

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

bool Item::isValid() const
{
    return !(storage().isEmpty() || product().isEmpty());
}

Item &Item::operator=(const Item &anotherItem)
{
    storage_ = anotherItem.storage();
    product_ = anotherItem.product();
    return *this;
}

QString Item::toString() const
{
    QString str("Item(%1;%2)");
    str = str.arg(storage())
            .arg(product());
    return str;
}

