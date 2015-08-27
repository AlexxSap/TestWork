#ifndef SALEHISTORYGENERATOR_H
#define SALEHISTORYGENERATOR_H

#include "SaleHistoryDay.h"
#include "stdlib.h"

class SaleHistoryGenerator
{
public:
    static QList<SaleHistoryDay> generateHistory(const Date &from,
                                                 const Date &to,
                                                 const int &storageNum,
                                                 const int &productNum,
                                                 const int &maxVal = 10000);
};

#endif // SALEHISTORYGENERATOR_H


