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
        const QString buffer=ts.readLine().trimmed();
        if(!rx.exactMatch(buffer))
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

int CsvFileReader::getProductId(DataBase &db, const QString productName) const
{
    QString request="select f_id from t_products where f_name='%1';";
    request=request.arg(productName);
    QSqlQuery query=db.read(request);
    int id=-1;
    query.next();
    if(!query.value(0).isNull())
    {
        id=query.value(0).toInt();
    }
    return id;
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

        file.reset();
        QTextStream ts(&file);
        ts.setCodec(QTextCodec::codecForName("Windows-1251"));
        while(!ts.atEnd())
        {
            const QStringList buffer=ts.readLine().trimmed().split(SEPARATOR);
            QString request;

            int id=getProductId(db, buffer.at(0));
            if(id<0)
            {
                request=QString("insert into t_products(f_name) "
                                "values('%1');");

                request=request.arg(buffer.at(0));
                if(!db.write(request))
                {
                    db.disconnect();
                    file.close();
                    return FileReader::DBError;
                }
            }

            id=getProductId(db, buffer.at(0));
            if(id<0)
            {
                db.disconnect();
                file.close();
                return FileReader::DBError;
            }

            request="insert into t_datas(f_product, f_date, f_sold, f_rest) "
                    "values('%1', '%2', %3, %4);";
            request=request.arg(id)
                    .arg(buffer.at(1))
                    .arg(buffer.at(2))
                    .arg(buffer.at(3));
            if(!db.write(request))
            {
                db.disconnect();
                file.close();
                return FileReader::DBError;
            }
        }
        db.disconnect();
    }
    file.close();
    //    emit ended();
    return error;
}
