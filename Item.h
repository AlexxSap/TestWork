#ifndef ITEM_H
#define ITEM_H

#include <QDate>
#include <QString>
#include <QMetaType>
#include <QDebug>

typedef double Amount;  // тип для выражения "кол-во товара"
typedef QDate Date;     // дата
typedef QString ID;     // текстовый идентификатор

class Item  // идентификатор товара - пара Склад и Артикул
{
private:
    ID storage_;
    ID product_;

public:   
    Item();
    Item(const ID &storage, const ID &product);
    Item(const Item &item);

    ID storage() const;
    ID product() const;

    bool isValid() const;

    Item& operator = (const Item &anotherItem);

    QString toString() const;

    bool operator != (const Item &other) const;
    bool operator == (const Item &other) const;
};

Q_DECLARE_METATYPE(Item)

inline QDebug operator << (QDebug debug, const Item &item)
{
   debug << item.toString();
   return debug;
}

inline uint qHash(const Item &item, uint seed)
{
    return qHash(item.storage(), seed) + qHash(item.product(), seed);
}

#endif // ITEM_H
