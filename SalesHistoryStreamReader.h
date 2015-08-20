#ifndef SALESHISTORYSTREAMREADER_H
#define SALESHISTORYSTREAMREADER_H

#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>

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

private:
    bool createTempItemsTable();
    void deleteTempItemsTable();

public:
    SalesHistoryStreamReader(const QList<Item> &items, const QString &dbName);

    bool open(const Date &from, const Date &to);
    bool next();
    SaleHistory current();
};

#endif // SALESHISTORYSTREAMREADER_H
