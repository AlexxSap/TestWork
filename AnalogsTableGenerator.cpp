#include "AnalogsTableGenerator.h"

AnalogsTable AnalogsTableGenerator::generateTable(const int &groupNum,
                                                  const int &analogsNum,
                                                  const int &maxProdNumFromSHG)
{
    const QString prodPrefix = "prod_";
    AnalogsTable table;
    QList<int> past;
    for(int group = 0; group < groupNum; group++)
    {
        int randProdNum = 0;
        while(true)
        {
            randProdNum =  rand() % maxProdNumFromSHG;
            if(!past.contains(randProdNum))
            {
                past.append(randProdNum);
                break;
            }
        }
        Analogs analogs(ID(prodPrefix + QString::number(randProdNum)));
        for(int an = 0; an < analogsNum; an++)
        {
            int randAnNum = 0;
            while(true)
            {
                randAnNum =  rand() % maxProdNumFromSHG;
                if(!past.contains(randAnNum))
                {
                    past.append(randAnNum);
                    break;
                }
            }
            analogs.addAnalog(ID(prodPrefix + QString::number(randAnNum)));
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

