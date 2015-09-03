#include "AnalogsWriter.h"

AnalogsWriter::AnalogsWriter(const DataBaseInfo &info)
    : db_(DataBase::getDataBase(info))

{
    db_->connect();
}

AnalogsWriter::~AnalogsWriter()
{
    db_->disconnect();
    delete db_;
}

bool AnalogsWriter::write(const AnalogsTable &table)
{
    if(!db_->isConnected())
    {
        return false;
    }
    return db_->insertValuesToTAnalogs(table);
}

bool AnalogsWriter::importFromFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QTextStream ts(&file);
    ts.setCodec(QTextCodec::codecForName("Windows-1251"));
    AnalogsTable table;
    while(!ts.atEnd())
    {
        const QString string = ts.readLine().trimmed();
        const QStringList list = string.split(";");
        if(list.count() == 0)
        {
            continue;
        }
        const ID mainAnalog = list.at(0);
        Analogs analogs(mainAnalog);
        foreach (const ID &a, list)
        {
            analogs.addAnalog(a);
        }
        table.addAnalogs(analogs);
    }
    file.close();

    return write(table);
}

