#include "CsvFileReader.h"

const char CsvFileReader::SEPARATOR = ';';

CsvFileReader::CsvFileReader():FileReader()
{

}

CsvFileReader::~CsvFileReader()
{

}

FileReader::Error CsvFileReader::watchFile(QFile &file) const
{
    if(file.size() == 0)
    {
        return FileReader::EmptyFile;
    }

    const QString dateFormat = QString("yyyy.MM.dd");
    QTextStream ts(&file);
    ts.setCodec(QTextCodec::codecForName("Windows-1251"));

    ///notes по-японски писать нельзя? а какие-нибудь символы типа двоеточия или амперсанта?
    QString rxPattern = QString("(^[?а-яА-ЯёЁa-zA-Z0-9_!]+)%1"                                  //наименование товара
                                "([?а-яА-ЯёЁa-zA-Z0-9_!]+)%1"                                  //наименование склада
                                "([0-9]{4}\.(0[1-9]|1[012])\.(0[1-9]|1[0-9]|2[0-9]|3[01]))%1"   //дата
                                "(\\d+(\.\\d{0,})?)%1"                                          //продали
                                "(\\d+(\.\\d{0,})?)"                                            //конечный остаток
                                );
    rxPattern = rxPattern.arg(SEPARATOR);
    const QRegExp rx(rxPattern);

    while(!ts.atEnd())
    {
        const QString buffer = ts.readLine().trimmed();
        if(!rx.exactMatch(buffer))
        {
            return FileReader::FileNotLoaded;
        }

        const QString date = rx.cap(3);
        if(!QDate::fromString(date,dateFormat).isValid())
        {
            return FileReader::FileNotLoaded;
        }
    }
    return FileReader::NoError;
}

int CsvFileReader::getProductId(DataBase &db, const QString &product, const QString &storage) const
{
    if(product.isEmpty() || storage.isEmpty())
    {
        return -1;
    }

    QSqlQuery query(db.getDB());
    query.prepare("select f_id from t_items where f_product = :product and f_storage = :storage;");
    query.bindValue(":product", product);
    query.bindValue(":storage", storage);
    if(!query.exec())
    {
        return -1;
    }

    if(!query.first())
    {
        QSqlQuery insQuery(db.getDB());
        insQuery.prepare("insert into t_items(f_product, f_storage) values(:product, :storage);");
        insQuery.bindValue(":product", product);
        insQuery.bindValue(":storage", storage);

        ///notes если операции как-то меняет состояние программы или дисковой системы и тд, то не надо
        /// их подряд писать через || или && или вообще как-то в if
        /// потому что порядок выполнения операций в с++ в данном случае не определён и может привести к очень хитрым багам.
        /// const методы, которые ничего не меняют, можно так использовать.
        if(!insQuery.exec() || !query.exec() || !query.first())
        {
            return -1;
        }

    }

    bool ok = false;
    int id = query.value(0).toInt(&ok);
    if(!ok)
    {
        return -1;
    }
    return id;
}

bool CsvFileReader::insertToDB(DataBase &db, const QStringList &data) const
{
    ///notes всегда, если используем методы типа QList::at от какой-то константы проверяем, что у списка хватает длинны
    /// иначе будет исключение. Неявное свойство data, что там точно будет сколько-то элементов в текущей версии никем не учитывается.
    const QString product=data.at(0);
    const QString storage=data.at(1);
    int id = getProductId(db, product, storage);

    if(id<0)
    {
        return false;
    }

    QSqlQuery query(db.getDB());
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

FileReader::Error CsvFileReader::readFromFile(const QString &fileName, DataBase &db)
{
    QFile file(fileName);
    FileReader::Error error = FileReader::NoError;
    if(!file.open(QIODevice::ReadOnly))
    {
        return FileReader::FileNotOpen;
    }

    error = watchFile(file);
    if(error != FileReader::NoError)
    {
        file.close();
        return error;
    }
    else
    {
        if(!db.isConnected())
        {
            file.close();
            return FileReader::DBError;
        }

        file.reset();
        QTextStream ts(&file);
        ts.setCodec(QTextCodec::codecForName("Windows-1251"));
        while(!ts.atEnd())
        {
            const QStringList buffer = ts.readLine().trimmed().split(SEPARATOR);
            if(!insertToDB(db, buffer))
            {
                file.close();
                return FileReader::DBError;

            }
        }
    }
    file.close();
    return error;
}
