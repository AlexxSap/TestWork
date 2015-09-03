#ifndef BENCHMARKFORCOMPARINGSQLITEANDMYSQL_H
#define BENCHMARKFORCOMPARINGSQLITEANDMYSQL_H

#include "DataBase.h"
#include "BenchmarkWriteRead.h"
#include "BenchmarkWriteReadWithAnalogs.h"

class BenchmarkForComparingSqliteAndMysql
{
public:
    static void run(const int &days,
                    const int &storages,
                    const int &products);

    static void runWithAnalogs(const int &days,
                               const int &storages,
                               const int &products,
                               const int &analogsGroupNum,
                               const int &analogsInGroupNum);

};

#endif // BENCHMARKFORCOMPARINGSQLITEANDMYSQL_H
