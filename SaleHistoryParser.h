#ifndef SALEHISTORYPARSER_H
#define SALEHISTORYPARSER_H

#include "SaleHistoryDay.h"


#include <sstream>
#include <string>

typedef std::vector<std::string> StdVector;

class SaleHistoryParser
{
private:
    QString pattern_;
    QString splitter_;
    std::string stdSplitter_;
    QString dateFormat_;
    bool isValid_;
    QRegExp rx_;
    Date strToDate(const QString &str);

public:
    SaleHistoryParser();
    void setSplitter(const QString &splitter);
    void setDateFormat(const QString &dateFormat);
    bool isValid() const;

    QList<SaleHistoryDay> parse(const QStringList &rawData);

    SaleHistoryDay parseString(const QString &raw);
    StdVector parseStdString(std::string raw);
};

#endif // SALEHISTORYPARSER_H
