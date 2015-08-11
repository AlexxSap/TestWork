#ifndef SALESHISTORYSTREAMREADER_H
#define SALESHISTORYSTREAMREADER_H

#include "SaleHistory.h"
#include "DataBase.h"
#include "SqliteSelByStorageAndProductDataSelector.h"
#include "SqliteSelByStorageAndProductAndDateDataSelector.h"

/// "потоковая" читалка товаров
class SalesHistoryStreamReader
{
private:
    QList<Item> items_;
    QSqlQuery query_;
    QString dbName_;
    int currentIndex_;
    QDate from_;
    QDate to_;

private:
    bool queryForNextItem();

public:
    /// собираемся читать items товары
    explicit SalesHistoryStreamReader(const QList<Item> &items);

    void setDbName(const QString &dbName);

    /// перед чтением указывает откуда и докуда будем читать
    /// from и to могут принимать значения Date(), в этом случае читаем от
    /// минимальной или до максимальной даты для каждого товара в отдельности
    /// возврашает false, если не удалось инициализировать чтение или вообще
    /// что-то пошло не так
    /// если from и to не Date(), то SaleHistory::from() == from и
    /// SalesHistory::to() == to для каждого элемента, который выдаёт
    /// SalesHistoryStreamReader::current()
    bool open(const Date &from, const Date &to);

    /// проверяем, есть ли ещё что читать в "потоке", возвращает
    /// false если нету, возвращает true если есть и переходит к этому элементу
    bool next();

    /// возвращает текущий SaleHistory в "потоке чтения"
    /// если в "потоке" ничего не осталось, то возвращает какой-то
    /// невалидный SaleHistory
    /// для всех items из конструктора SalesHistoryStreamReader::current
    /// должен выдать какой-то SaleHistory, независимо от существования товара в базе
    SaleHistory current();
};

#endif // SALESHISTORYSTREAMREADER_H
