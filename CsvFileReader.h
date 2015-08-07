#ifndef CSVFILEREADER_H
#define CSVFILEREADER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QByteArray>
#include <QList>
#include <QRegExp>
#include <QDate>
#include <QTextStream>
#include <QTextCodec>
#include <QSqlQuery>

#include "FileReader.h"
#include "DataBase.h"

class CsvFileReader : public FileReader
{
private:
    QString pattern_;
private:
//    FileReader::Error watchFile(QFile &file) const;
    int getProductId(DataBase &db, const QString &product, const QString &storage) const;
    bool insertToDB(DataBase &db, const QStringList &data) const;
    const QStringList getSplited(const QString &string) const;

public:
    CsvFileReader();
    ~CsvFileReader();

    FileReader::Error readFromFile(const QString &fileName, DataBase &db);

public:
    static const char SEPARATOR;
};

#endif // CSVFILEREADER_H
