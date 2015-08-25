#ifndef CSVFILE_H
#define CSVFILE_H

#include <QFile>
#include <QTextStream>
#include <QTextCodec>

#include "SaleHistoryDay.h"
#include "SaleHistoryParser.h"
#include "AnalogsTable.h"

class CsvFile
{
public:
    static bool write(const QList<SaleHistoryDay> &days, const QString &fileName);
    static bool write(const AnalogsTable &table, const QString &fileName);
    static QList<SaleHistoryDay> read(const QString &fileName);
};

#endif // CSVFILE_H
