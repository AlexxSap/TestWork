#include "CsvFileWriter.h"

const char CsvFileWriter::SEPARATOR=';';

CsvFileWriter::CsvFileWriter():FileWriter(){}

bool CsvFileWriter::writeToFileAllData(const QString &fileName, DataBase &db){
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
        return false;

    if(!db.connect())
        return false;

    db.beginRead();
    QSqlQuery *query=db.read();
    QString dateFormat=QString("yyyy.MM.dd");
    while(query->next()){
        QString temp("%1;%2;%3;%4");
        temp.arg(query->value(0).toString())
                .arg(query->value(1).toDate().toString(dateFormat))
                .arg(QString::number(query->value(2).toDouble(),'f',2))
                .arg(QString::number(query->value(3).toDouble(),'f',2));

        file.write(QString(temp+"\r\n").toLocal8Bit());
    }
    delete query;
    db.endRead();
    db.disconnect();
    file.close();
    return true;
}

bool CsvFileWriter::writeToFileByRequest(const QString &fileName, DataBase &db, const QString &request){
    return false;
}
