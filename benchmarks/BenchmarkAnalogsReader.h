#ifndef BENCHMARKANALOGSREADER_H
#define BENCHMARKANALOGSREADER_H

#include <QDebug>
#include <QElapsedTimer>

#include "tests/TestUtility.h"

#include "AnalogsTableGenerator.h"
#include "CsvFile.h"
#include "AnalogsWriter.h"
#include "AnalogsReader.h"

class BenchmarkAnalogsReader
{
public:
    static void run(const int &groupNum, const int &analogsNum);
};

#endif // BENCHMARKANALOGSREADER_H
