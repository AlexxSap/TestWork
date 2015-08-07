#include "CsvFileWriter.h"

const QString CsvFileWriter::SEPARATOR = ";";

CsvFileWriter::CsvFileWriter():FileWriter()
{
}

CsvFileWriter::~CsvFileWriter()
{
}

bool CsvFileWriter::writeToFile(const QString &fileName, DataBase &ds)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    QSqlQuery query = ds.get();

    if(!query.first())
    {
        file.close();
        return false;
    }

    const QString dateFormat = QString("yyyy.MM.dd");
    do
    {
        QString temp("%1;%2;%3;%4;%5");
        temp = temp.arg(query.value(0).toString())
                .arg(query.value(1).toString())
                .arg(query.value(2).toDate().toString(dateFormat))
                .arg(QString::number(query.value(3).toDouble(),'f',2))
                .arg(QString::number(query.value(4).toDouble(),'f',2));

        file.write(QString(temp + "\r\n").toLocal8Bit());
    } while(query.next());
    file.close();
    return true;
}
