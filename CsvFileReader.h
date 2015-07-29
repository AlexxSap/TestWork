#ifndef CSVFILEREADER_H
#define CSVFILEREADER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QByteArray>
#include <QList>
#include <QRegExp>

#include "FileReader.h"

class CsvFileReader : public FileReader
{
private:
    FileReader::Error watchFile(QFile &file) const;

public:
    CsvFileReader();
    ~CsvFileReader();

    FileReader::Error readFromFile(const QString &fileName) const;

public:
    static const char SEPARATOR;
};

#endif // CSVFILEREADER_H
