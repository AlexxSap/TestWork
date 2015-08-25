#ifndef ANALOGSTABLEGENERATOR_H
#define ANALOGSTABLEGENERATOR_H

#include "AnalogsTable.h"

class AnalogsTableGenerator
{
public:
    static AnalogsTable generateTable(const int &groupNum,
                               const int &analogsNum,
                               const QString &prodPrefix = "prod");
    static QList<ID> getRandomIdList(const AnalogsTable &table);
};

#endif // ANALOGSTABLEGENERATOR_H
