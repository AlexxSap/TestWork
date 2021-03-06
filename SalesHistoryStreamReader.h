#ifndef SALESHISTORYSTREAMREADER_H
#define SALESHISTORYSTREAMREADER_H

#include <QDebug>
#include <QElapsedTimer>

#include <sysinfoapi.h>
#include <psapi.h>
#include <windows.h>

#include "AnalogsReader.h"
#include "SaleHistory.h"
#include "DataBase.h"
#include "Analogs.h"

#include "RamTracker/Utils.h"

class SalesHistoryStreamReader
{
private:
    QList<Item> items_;
    QPointer<DataBase> db_;
    QSqlQuery query_;
    QDate from_;
    QDate to_;
    SaleHistory tempHistory_;
    bool isCanNext_;
    AnalogsTable analogsTable_;
    QHash<int, Item> itemsHashTable_;
    int limit_;
    int newLimit_;
    int offset_;
    int counter_;
    int memSize_;

private:
    bool fillTempItemsTable();
    bool fillInsLists(QVariantList &ids,
                      QVariantList &main);
    void addDayToTempHistory();
    bool isCanReturnHistory(const Item &item) const;
    void fetchAnalogsTable();
    void normalazeTempHistory();
    QHash<int, Item> insertNewItemsToTItems(const QList<Item> &newItems);
    bool nextQueryByOffset();
    bool nextRecord();
    bool bindLimitAndExec();
    long long currentMemoryUsage() const;

public:
    SalesHistoryStreamReader(const QList<Item> &items,
                             const DataBaseInfo &info);
    ~SalesHistoryStreamReader();

    bool open(const Date &from,
              const Date &to);
    bool next();
    SaleHistory current();
};

#endif // SALESHISTORYSTREAMREADER_H
