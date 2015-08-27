#ifndef SALESHISTORYSTREAMREADER_H
#define SALESHISTORYSTREAMREADER_H

#include <QDebug>

#include "AnalogsReader.h"
#include "SaleHistory.h"
#include "DataBase.h"
#include "Analogs.h"

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
    AnalogsTable analogsTable_;

private:
    bool createTempItemsTable();
    void remDuplicates(QVariantList &first, QVariantList &second, QVariantList &mainAn);
    void deleteTempItemsTable();
    void addDayToTempHistory();
    bool isCanReturnHistory(const Item &item);
    void loadAnalogsTable();
    QString buildSelectString();
    void normalazeTempHistory();

public:
    SalesHistoryStreamReader(const QList<Item> &items,
                             const QString &dbName);

    bool open(const Date &from,
              const Date &to);
    bool next();
    SaleHistory current();
};

#endif // SALESHISTORYSTREAMREADER_H
