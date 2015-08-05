#ifndef CSVFILEWRITER_H
#define CSVFILEWRITER_H

#include <QFile>
#include <QDate>
#include <QVariant>
#include <QTextStream>

#include "FileWriter.h"
#include "DataBase.h"

class CsvFileWriter : public FileWriter
{
public:
    CsvFileWriter();
    ~CsvFileWriter();

    bool writeToFile(const QString &fileName, DataBase &ds);

public:
    static const char SEPARATOR;
};

#endif // CSVFILEWRITER_H
