#ifndef CSVFILEWRITER_H
#define CSVFILEWRITER_H

#include <QFile>
#include <QDate>
#include <QVariant>
#include <QTextStream>

#include "FileWriter.h"
#include "DataSelector.h"

class CsvFileWriter : public FileWriter
{
public:
    CsvFileWriter();

    bool writeToFile(const QString &fileName, DataSelector &ds);

public:
    static const char SEPARATOR;
};

#endif // CSVFILEWRITER_H
