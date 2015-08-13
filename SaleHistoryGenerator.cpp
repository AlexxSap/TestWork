#include "SaleHistoryGenerator.h"

SaleHistoryGenerator::SaleHistoryGenerator()
    : maxVal_(10000),
      productPrefix_("prod_"),
      storagePrefix_("stor_")
{

}

void SaleHistoryGenerator::setMaxValue(const int &max)
{
    maxVal_ = max;
}

QList<SaleHistoryDay> SaleHistoryGenerator::generateHistory(const Date &from,
                                                            const Date &to,
                                                            const int storageNum,
                                                            const int productNum) const
{
    QList<SaleHistoryDay> buffer;

    for(int storIndex = 0; storIndex < storageNum; storIndex++)
    {
        for(int prodIndex = 0; prodIndex < productNum; prodIndex++)
        {
            Item item(storagePrefix_ + QString::number(storIndex),
                      productPrefix_ + QString::number(prodIndex));

            for(QDate dateIndex = from; dateIndex <= to; dateIndex = dateIndex.addDays(1))
            {
                const double sold = (rand() + 1) % maxVal_;
                const double rest = (rand() + 1) % maxVal_;
                SaleHistoryDay day(item, dateIndex, sold, rest);

                buffer.append(day);
            }
        }
    }
    return buffer;
}

