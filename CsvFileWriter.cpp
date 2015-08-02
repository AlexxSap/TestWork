#include "CsvFileWriter.h"

const char CsvFileWriter::SEPARATOR=';';

CsvFileWriter::CsvFileWriter():FileWriter()
{
}

CsvFileWriter::~CsvFileWriter()
{
}

bool CsvFileWriter::writeToFile(const QString &fileName, DataSelector &ds)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    QSqlQuery query=ds.get();

    const QString dateFormat=QString("yyyy.MM.dd");  
    while(query.next())
    {
        QString temp("%1;%2;%3;%4");
        temp=temp.arg(query.value(0).toString())
                .arg(query.value(1).toDate().toString(dateFormat))
                .arg(QString::number(query.value(2).toDouble(),'f',2))
                .arg(QString::number(query.value(3).toDouble(),'f',2));

        file.write(QString(temp+"\r\n").toLocal8Bit());
    }
    file.close();
    return true;
}
