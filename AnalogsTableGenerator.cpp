#include "AnalogsTableGenerator.h"

AnalogsTable AnalogsTableGenerator::generateTable(const int &groupNum,
                                                  const int &analogsNumMax,
                                                  const QString &prodPrefix)
{
    AnalogsTable table;
    for(int group = 0; group < groupNum; group++)
    {
        QString add = QString::number(group);
        Analogs analogs(ID(prodPrefix + add));
        int analogsCount = rand() % analogsNumMax;
        for(int an = 0; an < analogsCount; an++)
        {
            const ID id("_" + prodPrefix + add + "_" + QString::number(an));
            analogs.addAnalog(id);
        }

        table.addAnalogs(analogs);
    }
    return table;
}

QList<ID> AnalogsTableGenerator::getRandomIdList(const AnalogsTable &table)
{
    QList<ID> list;
    QList<Analogs> anList = table.toList();
    foreach (const Analogs an, anList)
    {
        const QList<ID> idList = an.toList();
        const int randIndex = ((rand() + 1) % idList.count());
        list.append(idList.at(randIndex));
    }

    return list;
}

