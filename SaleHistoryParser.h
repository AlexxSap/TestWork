#ifndef SALEHISTORYPARSER_H
#define SALEHISTORYPARSER_H

#include "SaleHistoryDay.h"

class SaleHistoryParser
{
private:
    QString pattern_;
    QString splitter_;
    QString dateFormat_;
    bool isValid_;

public:
    SaleHistoryParser();
    void setSplitter(const QString &splitter);
    void setDateFormat(const QString &dateFormat);
    bool isValid() const;

    QList<SaleHistoryDay> parse(const QStringList &rawData);
};

#endif // SALEHISTORYPARSER_H
