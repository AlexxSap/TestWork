#include "AnalogsReader.h"

bool AnalogsReader::createTempIdTable()
{
    QSqlQuery query = db_.getAssociatedQuery();
    db_.beginTransaction();
    if(!query.exec("create temporary table if not exists t_temp_idmain("
                   "f_main text, "
                   "f_id text);"))
    {
        db_.rollbackTransaction();
        return false;
    }

    if(!query.exec("create temporary table if not exists t_temp_ids("
                   "f_id text not null);"))
    {
        db_.rollbackTransaction();
        return false;
    }
    db_.commitTransaction();
    return true;
}

void AnalogsReader::dropTempIdTable()
{
    QSqlQuery query = db_.getAssociatedQuery();
    db_.beginTransaction();
    query.exec("drop table if exists t_temp_ids;");
    query.exec("drop table if exists t_temp_idmain;");
    db_.commitTransaction();
}

bool AnalogsReader::fillTempIdTable(const QList<ID> IdList)
{
    QSqlQuery query = db_.getAssociatedQuery();
    query.prepare("insert into t_temp_ids (f_id) values (?);");
    QVariantList lst ;
    foreach (const ID id, IdList)
    {
        lst << id;
    }
    query.addBindValue(lst);

    db_.beginTransaction();
    if(!query.execBatch())
    {
        db_.rollbackTransaction();
        return false;
    }
    db_.commitTransaction();

    db_.beginTransaction();
    if(!query.exec("insert into t_temp_idmain(f_main, f_id) "
                   "select t_analogs.f_main, t_analogs.f_analog "
                   "from t_temp_ids left outer join t_analogs "
                   "on t_analogs.f_analog = t_temp_ids.f_id;"))
    {
        qInfo() << query.lastError().text();
        qInfo() << query.lastQuery();
        db_.rollbackTransaction();
        return false;
    }
    db_.commitTransaction();

    return true;
}

AnalogsTable AnalogsReader::getTable()
{
    QSqlQuery query = db_.getAssociatedQuery();
    query.setForwardOnly(true);

    if(!query.exec("select t_analogs.f_main, t_analogs.f_analog "
                   "from t_temp_idmain left outer join t_analogs "
                   "on t_analogs.f_main = t_temp_idmain.f_main "
                   "where t_temp_idmain.f_main is not null "
                   "order by t_analogs.f_main;"))
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
    :db_(dbName, "con_for_analogs_reader")
{

}


AnalogsTable AnalogsReader::read(const QList<ID> IdList)
{
    AnalogsTable table;
    if(IdList.isEmpty())
    {
        return table;
    }
    if(!createTempIdTable())
    {
        return table;
    }
    if(!fillTempIdTable(IdList))
    {
        return table;
    }

    table = getTable();

    dropTempIdTable();
    return table;
}

