#include "CsvFileReader.h"

const char CsvFileReader::SEPARATOR=';';

CsvFileReader::CsvFileReader():FileReader()
{

}

CsvFileReader::~CsvFileReader()
{

}

FileReader::Error CsvFileReader::watchFile(QFile &file) const
{
    if(file.size()==0)
    {
        return FileReader::EmptyFile;
    }

    file.reset();
    const QString dateFormat=QString("yyyy.MM.dd");
    QTextStream ts(&file);
    ts.setCodec(QTextCodec::codecForName("Windows-1251"));

    QString rxPattern=QString("(^[?а-яА-ЯёЁa-zA-Z0-9_!]+)%1"
                              "([0-9]{4}\.(0[1-9]|1[012])\.(0[1-9]|1[0-9]|2[0-9]|3[01]))%1"
                              "(\\d+(\.\\d{0,})?)%1"
                              "(\\d+(\.\\d{0,})?)");
    rxPattern=rxPattern.arg(SEPARATOR);
    const QRegExp rx(rxPattern);

    while(!ts.atEnd())
    {
        const QString strBuffer=ts.readLine().trimmed();
        if(!rx.exactMatch(strBuffer))
        {
            return FileReader::FileNotLoaded;
        }

        const QString date=rx.cap(2);
        if(!QDate::fromString(date,dateFormat).isValid())
        {
            return FileReader::FileNotLoaded;
        }
    }
    return FileReader::NoError;
}

FileReader::Error CsvFileReader::readFromFile(const QString &fileName, DataBase &db)
{
    QFile file(fileName);
    FileReader::Error error=FileReader::NoError;
    if(!file.open(QIODevice::ReadOnly))
    {
        return FileReader::FileNotOpen;
    }

//    emit started();
    error=watchFile(file);
    if(error!=FileReader::NoError)
    {
        file.close();
//        emit canceled();
        return error;
    }
    else
    {
        if(!db.connect())
        {
            file.close();
//            emit canceled();
            return FileReader::DBError;
        }

        db.beginWrite();

        file.reset();
//        while(!file.atEnd())
//        {
//            QStringList buffer=QString(file.readLine().trimmed()).split(SEPARATOR);

//            db.write();

//        }
        db.endWrite();
        db.disconnect();
    }

    file.close();
//    emit ended();
    return error;
}
