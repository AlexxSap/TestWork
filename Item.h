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
};

Q_DECLARE_METATYPE(Item)

bool operator!= (const Item &left, const Item &right);
bool operator== (const Item &left, const Item &right);

inline QDebug operator << (QDebug debug, const Item &item)
{
   debug << item.toString();
   return debug;
}

#endif // ITEM_H
