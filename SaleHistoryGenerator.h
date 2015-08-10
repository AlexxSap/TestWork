#ifndef SALEHISTORYGENERATOR_H
#define SALEHISTORYGENERATOR_H

#include "SaleHistoryDay.h"
#include "stdlib.h"

class SaleHistoryGenerator
{
private:
    int maxVal_;
    QString productPrefix_;
    QString storagePrefix_;

public:
    SaleHistoryGenerator();

    void setMaxValue(const int &max);

    QList<SaleHistoryDay> generateHistory(const Date &from,
                                          const Date &to,
                                          const int storageNum,
                                          const int productNum) const;
};

#endif // SALEHISTORYGENERATOR_H


