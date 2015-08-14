#ifndef BENCHMARKWRITEREAD_H
#define BENCHMARKWRITEREAD_H

#include <QDebug>
#include <QElapsedTimer>

#include "SaleHistoryDay.h"
#include "tests/TestUtility.h"
#include "SaleHistoryGenerator.h"
#include "SaleHistoryWriter.h"
#include "SalesHistoryStreamReader.h"

class BenchmarkWriteRead
{
public:
    static void run(const int &days, const int &storages, const int &products);
};

#endif // BENCHMARKWRITEREAD_H
