#include "AnalogsReader.h"

bool AnalogsReader::fillTempIdTable(const QList<ID> &IdList)
{
    QSqlQuery query = db_->associatedQuery();
    query.prepare("insert into tTempIds (fId) values (?);");
    QVariantList lst ;
    foreach (const ID &id, IdList)
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
    if(!query.exec("insert into tTempIdMain(fMain, fId) "
                   "select tAnalogs.fMain, tAnalogs.fAnalog "
                   "from tTempIds left outer join tAnalogs "
                   "on tAnalogs.fAnalog = tTempIds.fId;"))
    {
        db_->rollbackTransaction();
        return false;
    }
    db_->commitTransaction();

    return true;
}

AnalogsTable AnalogsReader::fetchTable()
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
        return AnalogsTable();
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

AnalogsReader::AnalogsReader(const DataBaseInfo &info)
    : db_(DataBase::getDataBase(info,
                                "con_for_analogs_reader"))
{
    db_->connect();
}

AnalogsReader::~AnalogsReader()
{
    db_->disconnect();
    delete db_;
}

AnalogsTable AnalogsReader::fetch(const QList<ID> &IdList)
{
    AnalogsTable table;
    if(IdList.isEmpty())
    {
        return AnalogsTable();
    }
    if(!db_->createTempTableForAnalogsReader())
    {
        return AnalogsTable();
    }
    if(!fillTempIdTable(IdList))
    {
        return AnalogsTable();
    }

    table = fetchTable();

    db_->dropTempTableForAnalogsReader();
    return table;
}

