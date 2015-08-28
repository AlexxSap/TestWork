#include "Item.h"
#include "ItemImpl.h"

bool operator != (const Item &left, const Item &right)
{
    return left.product() != right.product() || left.storage() != right.storage();
}

bool operator == (const Item &left, const Item &right)
{
    return left.product() == right.product() && left.storage() == right.storage();
}

Item::Item()
    :impl_(new ItemImpl())
{

}

Item::Item(const ID &storage, const ID &product)
    :impl_(new ItemImpl(storage, product))
{

}

Item::Item(const Item &other)
    :impl_(other.impl_)
{

}

ID Item::storage() const
{
    return impl_->storage_;
}

ID Item::product() const
{
    return impl_->product_;
}

bool Item::isValid() const
{
     return !(storage().isEmpty() || product().isEmpty());
}

Item &Item::operator =(const Item &other)
{
    impl_->storage_ = other.storage();
    impl_->product_ = other.product();

    return *this;
}

QString Item::toString() const
{
    QString str("Item(%1;%2)");
    str = str.arg(storage())
            .arg(product());
    return str;
}
