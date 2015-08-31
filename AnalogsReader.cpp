#include "AnalogsReader.h"

bool AnalogsReader::fillTempIdTable(const QList<ID> IdList)
{
    QSqlQuery query = db_->getAssociatedQuery();
    query.prepare("insert into t_temp_ids (f_id) values (?);");
    QVariantList lst ;
    foreach (const ID id, IdList)
    {
        lst << id;
    }
    query.addBindValue(lst);

    db_->beginTransaction();
    if(!query.execBatch())
    {
        db_->rollbackTransaction();
        return false;
    }
    db_->commitTransaction();

    db_->beginTransaction();
    if(!query.exec("insert into t_temp_idmain(f_main, f_id) "
                   "select t_analogs.f_main, t_analogs.f_analog "
                   "from t_temp_ids left outer join t_analogs "
                   "on t_analogs.f_analog = t_temp_ids.f_id;"))
    {
        qInfo() << query.lastError().text();
        qInfo() << query.lastQuery();
        db_->rollbackTransaction();
        return false;
    }
    db_->commitTransaction();

    return true;
}

AnalogsTable AnalogsReader::getTable()
{
    QSqlQuery query = db_->queryForAnalogsReader();
    if(!query.exec())
    {
        return AnalogsTable();
    }

    AnalogsTable table;
    ID prevMain;
    Analogs analogs;

    if(query.next())
    {
        const ID main = query.value(0).toString();
        const ID analog = query.value(1).toString();
        analogs = Analogs (main);
        analogs << analog;
        prevMain = main;
    }
    else
    {
        return table;
    }

    if(!query.next())
    {
        table << analogs;
        return table;
    }

    do
    {
        const ID main = query.value(0).toString();
        const ID analog = query.value(1).toString();

        if(main == prevMain)
        {
            analogs << analog;
        }
        else
        {
            prevMain = main;
            table << analogs;
            analogs = Analogs(main);
            analogs << analog;

        }

    } while(query.next());
    if(analogs.isValid())
    {
        table << analogs;
    }

    return table;
}

AnalogsReader::AnalogsReader(const QString &dbName)
    : db_(DataBase::getDataBase(dbName,
                      DataBase::SQLITE,
                      "",
                      "",
                      "con_for_analogs_reader"))
{
    db_->connect();
}

AnalogsReader::~AnalogsReader()
{
    db_->disconnect();
    delete db_;
}

AnalogsTable AnalogsReader::read(const QList<ID> IdList)
{
    AnalogsTable table;
    if(IdList.isEmpty())
    {
        return table;
    }
    if(!db_->createTempTableForAnalogsReader())
    {
        return table;
    }
    if(!fillTempIdTable(IdList))
    {
        return table;
    }

    table = getTable();

    db_->dropTempTableForAnalogsReader();
    return table;
}

