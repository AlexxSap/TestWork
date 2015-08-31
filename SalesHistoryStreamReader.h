#ifndef SALESHISTORYSTREAMREADER_H
#define SALESHISTORYSTREAMREADER_H

#include <QDebug>
#include <QElapsedTimer>

#include "AnalogsReader.h"
#include "SaleHistory.h"
#include "DataBase.h"
#include "Analogs.h"

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


private:
    bool fillTempItemsTable();
    void fillInsLists(QVariantList &stor,
                      QVariantList &prod,
                      QVariantList &main) const;
    void addDayToTempHistory();
    bool isCanReturnHistory(const Item &item) const;
    void loadAnalogsTable();
    QString buildSelectString() const;
    void normalazeTempHistory();

public:
    SalesHistoryStreamReader(const QList<Item> &items,
                             const QString &dbName);
    ~SalesHistoryStreamReader();

    bool open(const Date &from,
              const Date &to);
    bool next();
    SaleHistory current();
};

#endif // SALESHISTORYSTREAMREADER_H
