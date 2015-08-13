#ifndef SALEHISTORYWRITER_H
#define SALEHISTORYWRITER_H

#include "SaleHistoryDay.h"
#include "DataBase.h"

class SaleHistoryWriter
{
private:
    DataBase db_;

public:
    explicit SaleHistoryWriter(const QString &dbName);
    bool write(const QList<SaleHistoryDay> &days);
};

#endif // SALEHISTORYWRITER_H
