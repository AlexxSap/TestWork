#ifndef BENCHMARKWRITEREADWITHANALOGS_H
#define BENCHMARKWRITEREADWITHANALOGS_H

#include <QString>

#include "tests/TestUtility.h"

#include "AnalogsWriter.h"
#include "AnalogsReader.h"
#include "AnalogsTableGenerator.h"

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
