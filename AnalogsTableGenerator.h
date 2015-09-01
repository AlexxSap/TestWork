#ifndef ANALOGSTABLEGENERATOR_H
#define ANALOGSTABLEGENERATOR_H

#include "AnalogsTable.h"

class AnalogsTableGenerator
{
public:
    static AnalogsTable generateTable(const int &groupNum,
                                      const int &analogsNum,
                                      const int &maxProdNumFromSHG = 1000000);

    static QList<ID> generateRandomIdList(const AnalogsTable &table);
};

#endif // ANALOGSTABLEGENERATOR_H
