#include "AnalogsWriter.h"

AnalogsWriter::AnalogsWriter(const QString &dbName)
    :db_(dbName)
{

}

bool AnalogsWriter::write(const AnalogsTable &table)
{
    const QList<Analogs> anList = table.toList();
    if(anList.isEmpty())
    {
        return true;
    }

    QSqlQuery query = db_.getAssociatedQuery();
    query.prepare("insert into t_analogs (f_main, f_analog) "
                  "values(?, ?);");

    QVariantList mains;
    QVariantList anVarLits;

    foreach(const Analogs an, anList)
    {
        const ID mainAn = an.mainAnalog();
        const QList<ID> analogs = an.toList();
//        mains << mainAn;
//        anVarLits << mainAn;
        foreach (const ID id, analogs)
        {
            mains << mainAn;
            anVarLits << id;
        }
    }

    query.addBindValue(mains);
    query.addBindValue(anVarLits);

    db_.beginTransaction();
    if(!query.execBatch())
    {
        qInfo() << query.lastError().text();
        qInfo() << query.executedQuery();
        db_.rollbackTransaction();
        return false;
    }
    db_.commitTransaction();

//    if(!query.exec("select f_main, f_analog from t_analogs;"))
//    {
//        qInfo() << query.lastError().text();
//        qInfo() << query.lastQuery();
//        db_.rollbackTransaction();
//        return false;
//    }
//    while(query.next())
//    {
//        qInfo() << query.value(0).toString() << query.value(1).toString();
//    }

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

