#ifndef SALEHISTORYPARSER_H
#define SALEHISTORYPARSER_H

#include "SaleHistoryDay.h"

typedef std::vector<std::string> StdVector;

class SaleHistoryParser
{
private:
    QString splitter_;
    QString dateFormat_;
    bool isValid_;
    Date strToDate(const QString &str);

public:
    SaleHistoryParser();
    bool isValid() const;

    QList<SaleHistoryDay> parse(const QStringList &rawData);

    SaleHistoryDay parseString(const QString &raw);
};

#endif // SALEHISTORYPARSER_H
