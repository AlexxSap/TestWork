#include "ItemImpl.h"


ItemImpl::ItemImpl()
    : storage_(),
      product_()
{

}

ItemImpl::ItemImpl(const ID &storage, const ID &product)
    : storage_(storage),
      product_(product)
{

}

ItemImpl::ItemImpl(const ItemImpl &other)
    : QSharedData(other),
      storage_(other.storage_),
      product_(other.product_)
{

}
