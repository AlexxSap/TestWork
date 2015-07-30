#ifndef CSVFILEWRITER_H
#define CSVFILEWRITER_H

#include <QFile>
#include <QDate>

#include "FileWriter.h"

class CsvFileWriter : public FileWriter
{
public:
    CsvFileWriter();

    bool writeToFileAllData(const QString &fileName, DataBase &db);
    bool writeToFileByRequest(const QString &fileName, DataBase &db, const QString &request);

public:
    static const char SEPARATOR;
};

#endif // CSVFILEWRITER_H
