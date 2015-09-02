#ifndef BENCHMARKFORCOMPARINGSQLITEANDMYSQL_H
#define BENCHMARKFORCOMPARINGSQLITEANDMYSQL_H

#include "DataBase.h"
#include "BenchmarkWriteRead.h"

class BenchmarkForComparingSqliteAndMysql
{
public:
    static void run(const int &days,
                    const int &storages,
                    const int &products);

};

#endif // BENCHMARKFORCOMPARINGSQLITEANDMYSQL_H
