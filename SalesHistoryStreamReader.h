#ifndef SALESHISTORYSTREAMREADER_H
#define SALESHISTORYSTREAMREADER_H

#include <QDebug>

#include "SaleHistory.h"
#include "DataBase.h"

class SalesHistoryStreamReader
{
private:
    QList<Item> items_;
    DataBase db_;
    QSqlQuery query_;
    QDate from_;
    QDate to_;
    SaleHistory tempHistory_;
    bool isCanNext_;

private:
    bool createTempItemsTable();
    void deleteTempItemsTable();
    void addDayToTempHistory();

public:
    SalesHistoryStreamReader(const QList<Item> &items, const QString &dbName);

    bool open(const Date &from, const Date &to, const bool &analogs = false);
    bool next();
    SaleHistory current();
};

#endif // SALESHISTORYSTREAMREADER_H
