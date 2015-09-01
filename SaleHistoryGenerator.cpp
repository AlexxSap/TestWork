#include "SaleHistoryGenerator.h"

QList<SaleHistoryDay> SaleHistoryGenerator::generateHistory(const Date &from,
                                                            const Date &to,
                                                            const int &storageNum,
                                                            const int &productNum,
                                                            const int &maxVal)
{
    if(!from.isValid() || !to.isValid())
    {
        return QList<SaleHistoryDay>();
    }

    const QString productPrefix("prod_");
    const QString storagePrefix("stor_");

    QList<SaleHistoryDay> buffer;
    for(int storIndex = 0; storIndex < storageNum; storIndex++)
    {
        for(int prodIndex = 0; prodIndex < productNum; prodIndex++)
        {
            const Item item(storagePrefix + QString::number(storIndex),
                      productPrefix + QString::number(prodIndex));

            for(QDate dateIndex = from; dateIndex <= to; dateIndex = dateIndex.addDays(1))
            {
                const double sold = (rand() + 1) % maxVal;
                const double rest = (rand() + 1) % maxVal;
                const SaleHistoryDay day(item, dateIndex, sold, rest);

                buffer.append(day);
            }
        }
    }
    return buffer;
}
