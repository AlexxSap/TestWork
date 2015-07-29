#ifndef CSVFILEREADER_H
#define CSVFILEREADER_H

#include <QObject>

#include "FileReader.h"

class CsvFileReader : public FileReader
{
public:
    CsvFileReader();
    ~CsvFileReader();

    FileReader::Error readFromFile(const QString &fileName);
};

#endif // CSVFILEREADER_H
