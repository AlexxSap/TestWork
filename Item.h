#ifndef ITEM_H
#define ITEM_H

#include <QDate>
#include <QString>
#include <QMetaType>

typedef double Amount;  // тип для выражения "кол-во товара"
typedef QDate Date;     // дата
typedef QString ID;     // текстовый идентификатор

class Item  // идентификатор товара - пара Склад и Артикул
{

private:
    ID storage_;
    ID product_;

private:
    Item(); //не может быть пустого Item

public:   
    Item(const ID &storage, const ID &product);

    ID storage() const;
    ID product() const;

    Item& operator = (const Item &anotherItem);
};

//Q_DECLARE_METATYPE(Item)
//или объявлять Item::Item() открытым и использовать Item в шаблоне
//или объявлять Item::Item() закрытым и спользовать строки
//выбрал второе

bool operator != (const Item& left, const Item& right);
bool operator == (const Item& left, const Item& right);

#endif // ITEM_H
