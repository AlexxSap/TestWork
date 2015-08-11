#ifndef SALEHISTORYWRITER_H
#define SALEHISTORYWRITER_H

#include "SaleHistoryDay.h"
#include "DataBase.h"

class SaleHistoryWriter
{
private:
    DataBase db_;

private:
    int getItemId(const ID &product, const ID &storage);

public:
    explicit SaleHistoryWriter(const QString &dbName);
    bool write(const QList<SaleHistoryDay> &days);
};

#endif // SALEHISTORYWRITER_H
