#ifndef ITEMIMPL_H
#define ITEMIMPL_H

#include <QSharedData>
#include <QString>
#include <QMetaType>

typedef QString ID;

class ItemImpl : public QSharedData
{
public:
    ID storage_;
    ID product_;

public:
    ItemImpl();
    ItemImpl(const ID &storage, const ID &product);
    ItemImpl(const ItemImpl &other);
};

Q_DECLARE_TYPEINFO(ItemImpl, Q_MOVABLE_TYPE);

#endif // ITEMIMPL_H
