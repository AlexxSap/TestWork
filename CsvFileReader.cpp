#include "CsvFileReader.h"

const char CsvFileReader::SEPARATOR = ';';

CsvFileReader::CsvFileReader():
    FileReader(),
    pattern_("(^[^;]+|\"[^\"]+\")%1"                                  //наименование товара
             "([^;]+|\"[^\"]+\")%1"                                  //наименование склада
             "([0-9]{4}\.(0[1-9]|1[012])\.(0[1-9]|1[0-9]|2[0-9]|3[01]))%1"   //дата
             "(\\d+(\.\\d{0,})?)%1"                                          //продали
             "(\\d+(\.\\d{0,})?)")                                            //конечный остаток
{
    pattern_=pattern_.arg(SEPARATOR);
}

CsvFileReader::~CsvFileReader()
{

}

int CsvFileReader::getProductId(DataBase &db, const QString &product, const QString &storage) const
{
    if(product.isEmpty() || storage.isEmpty())
    {
        return -1;
    }

    QSqlQuery query=db.getAssociatedQuery();
    query.prepare("select f_id from t_items where f_product = :product and f_storage = :storage;");
    query.bindValue(":product", product);
    query.bindValue(":storage", storage);
    if(!query.exec())
    {
        return -1;
    }

    if(!query.first())
    {
        QSqlQuery insQuery=db.getAssociatedQuery();
        insQuery.prepare("insert into t_items(f_product, f_storage) values(:product, :storage);");
        insQuery.bindValue(":product", product);
        insQuery.bindValue(":storage", storage);

        bool isOk=insQuery.exec();
        if(isOk)
        {
            isOk=query.exec();
        }
        if(isOk)
        {
            isOk=query.first();
        }

        if(!isOk)
        {
            return -1;
        }
    }

    bool isOk = false;
    int id = query.value(0).toInt(&isOk);
    if(!isOk)
    {
        return -1;
    }
    return id;
}

bool CsvFileReader::insertToDB(DataBase &db, const QStringList &data) const
{
    //Неявное свойство data, что там точно будет сколько-то элементов, обеспечивается при разделении
    //строки методом getSplited
    const QString product = data.at(0);
    const QString storage = data.at(1);
    int id = getProductId(db, product, storage);

    if(id < 0)
    {
        return false;
    }

    QSqlQuery query = db.getAssociatedQuery();
    query.prepare("insert into t_datas(f_item, f_date, f_sold, f_rest) values(:id, :date, :sold, :rest);");
    query.bindValue(":id", id);
    query.bindValue(":date", data.at(2));
    query.bindValue(":sold", data.at(3));
    query.bindValue(":rest", data.at(4));
    if(!query.exec())
    {
        return false;
    }
    return true;
}

const QStringList CsvFileReader::getSplited(const QString &string) const
{
    QStringList res;
    const QRegExp rx(pattern_);

    if(!rx.exactMatch(string))
    {
        return QStringList();
    }
    //если уж дошли до сюда, то список captured содержит всё что нам нужно
    res << rx.cap(1)
        << rx.cap(2)
        << rx.cap(3)
        << rx.cap(6)
        << rx.cap(8);

    return res;
}

FileReader::Error CsvFileReader::readFromFile(const QString &fileName, DataBase &db)
{
    QFile file(fileName);
    FileReader::Error error = FileReader::NoError;
    if(!file.open(QIODevice::ReadOnly))
    {
        return FileReader::FileNotOpen;
    }
    if(file.size() == 0)
    {
        return FileReader::EmptyFile;
    }

    if(!db.isConnected())
    {
        file.close();
        return FileReader::DBError;
    }

    file.reset();
    QTextStream ts(&file);
    ts.setCodec(QTextCodec::codecForName("Windows-1251"));
    db.beginTransaction();
    while(!ts.atEnd())
    {
        QString bufString = ts.readLine().trimmed();
        const QStringList buffer = getSplited(bufString);
        if(buffer.isEmpty())
        {
            file.close();
            db.rollbackTransaction();
            return FileReader::FileNotLoaded;;
        }
        if(!insertToDB(db, buffer))
        {
            file.close();
            db.rollbackTransaction();
            return FileReader::DBError;
        }
    }
    db.commitTransaction();

    file.close();
    return error;
}
