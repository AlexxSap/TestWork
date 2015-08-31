#include "AnalogsWriter.h"

AnalogsWriter::AnalogsWriter(const QString &dbName)
    : db_(DataBase::getDataBase(dbName))

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
    const QList<Analogs> anList = table.toList();
    if(anList.isEmpty())
    {
        return true;
    }

    QSqlQuery query = db_->getAssociatedQuery();
    query.prepare("insert into t_analogs (f_main, f_analog) "
                  "values(?, ?);");

    QVariantList mains;
    QVariantList anVarLits;

    foreach(const Analogs an, anList)
    {
        const ID mainAn = an.mainAnalog();
        const QList<ID> analogs = an.toList();
        foreach (const ID id, analogs)
        {
            mains << mainAn;
            anVarLits << id;
        }
    }

    query.addBindValue(mains);
    query.addBindValue(anVarLits);

    db_->beginTransaction();
    if(!query.execBatch())
    {
        qInfo() << query.lastError().text();
        qInfo() << query.executedQuery();
        db_->rollbackTransaction();
        return false;
    }
    db_->commitTransaction();
    return true;
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
        foreach (ID a, list)
        {
            analogs.addAnalog(a);
        }
        table.addAnalogs(analogs);
    }
    file.close();

    return write(table);
}

