#ifndef BENCHMARKWRITEREAD_H
#define BENCHMARKWRITEREAD_H

#include <QDebug>
#include <QElapsedTimer>
#include <QTest>

#include "SaleHistoryDay.h"
#include "tests/TestUtility.h"
#include "SaleHistoryGenerator.h"
#include "SaleHistoryWriter.h"
#include "SalesHistoryStreamReader.h"
#include "CsvFile.h"
#include "RamTracker/Utils.h"

class BenchmarkWriteRead
{
private:
    static QList<Item> genRandomItemList(const int storages,
                                  const int products);
public:
    static void run(const int &days, const int &storages, const int &products);
    static void runForBuffer(const int bufferSize);
};

#endif // BENCHMARKWRITEREAD_H
