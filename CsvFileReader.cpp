#include "CsvFileReader.h"

const char CsvFileReader::SEPARATOR=';';

CsvFileReader::CsvFileReader():FileReader(){}

CsvFileReader::~CsvFileReader(){}

FileReader::Error CsvFileReader::watchFile(QFile &file) const{
    if(file.size()==0)
        return FileReader::EmptyFile;

    while(!file.atEnd()){
        QString strBuffer=QString(file.readLine().trimmed());

        QString dateFormat=QString("yyyy.MM.dd");
        QString rxPattern=QString("(^[?a-zA-Z0-9_!]+)%1"
                                  "([0-9]{4}\.(0[1-9]|1[012])\.(0[1-9]|1[0-9]|2[0-9]|3[01]))%1"
                                  "(\\d+(\.\\d{0,})?)%1"
                                  "(\\d+(\.\\d{0,})?)");

        rxPattern=rxPattern.arg(SEPARATOR);

        QRegExp rx(rxPattern);
        if(!rx.exactMatch(strBuffer))
            return FileReader::FileNotLoaded;

        QString date=rx.cap(2);
        if(!QDate::fromString(date,dateFormat).isValid())
            return FileReader::FileNotLoaded;
    }
    return FileReader::NoError;
}

FileReader::Error CsvFileReader::readFromFile(const QString &fileName) const{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return FileReader::FileNotOpen;

    FileReader::Error error=watchFile(file);
    if(error!=FileReader::NoError){
        file.close();
        return error;
    }
    else{
        //work with file without error

    }

    file.close();
    return error;
}
