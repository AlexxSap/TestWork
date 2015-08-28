#ifndef ITEM_H
#define ITEM_H

#include <QSharedDataPointer>
#include <QDebug>
#include <QDate>
#include <QSharedData>

#include "ItemImpl.h"

typedef double Amount;
typedef QDate Date;
typedef QString ID;

class ItemImpl;

class Item
{
private:
    QSharedDataPointer<ItemImpl> impl_;

public:
    Item();
    Item(const ID &storage, const ID &product);
    Item(const Item &other);

    ID storage() const;
    ID product() const;

    bool isValid() const;
    Item& operator = (const Item &other);
    QString toString() const;
};

Q_DECLARE_METATYPE(Item)

bool operator != (const Item &left, const Item &right);
bool operator == (const Item &left, const Item &right);

inline QDebug& operator << (QDebug &debug, const Item &item)
{
   debug << item.toString();
   return debug;
}

#endif // ITEM_H
