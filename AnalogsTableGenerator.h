#ifndef ANALOGSTABLEGENERATOR_H
#define ANALOGSTABLEGENERATOR_H

#include "AnalogsTable.h"

class AnalogsTableGenerator
{
public:
    AnalogsTableGenerator();

    AnalogsTable generateTable(const int &groupNum,
                               const int &analogsNumMax,
                               const QString &prodPrefix = "prod");
};

#endif // ANALOGSTABLEGENERATOR_H
