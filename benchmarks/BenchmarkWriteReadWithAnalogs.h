#ifndef BENCHMARKWRITEREADWITHANALOGS_H
#define BENCHMARKWRITEREADWITHANALOGS_H

#include <QString>
#include <QElapsedTimer>

#include "tests/TestUtility.h"

#include "AnalogsWriter.h"
#include "AnalogsReader.h"
#include "AnalogsTableGenerator.h"
#include "SaleHistoryGenerator.h"
#include "CsvFile.h"
#include "SaleHistoryWriter.h"
#include "RamTracker/Utils.h"
#include "SalesHistoryStreamReader.h"

class BenchmarkWriteReadWithAnalogs
{
public:
    static void run(const int &days,
                    const int &storages,
                    const int &products,
                    const int &analogsGroupNum,
                    const int &analogsInGroupNum);
};

#endif // BENCHMARKWRITEREADWITHANALOGS_H
