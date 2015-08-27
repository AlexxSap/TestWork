#include "SalesHistoryStreamReader.h"

SalesHistoryStreamReader::SalesHistoryStreamReader(const QList<Item> &items,
                                                   const QString &dbName)
    :items_(items),
      db_(dbName),
      query_(),
      from_(),
      to_(),
      tempHistory_(),
      isCanNext_(false),
      analogsTable_()
{

}

bool SalesHistoryStreamReader::createTempItemsTable()
{
    QSqlQuery query = db_.getAssociatedQuery();
    db_.beginTransaction();
    if(!query.exec("create temp table t_temp_items("
                   "f_storage text not null, "
                   "f_product text not null, "
                   "unique(f_storage, f_product));"))
    {
        qInfo()  << "cannot create temp table";
        db_.rollbackTransaction();
        return false;
    }
    db_.commitTransaction();

    query.clear();

    QVariantList storageList;
    QVariantList productList;

    foreach (const Item &item, items_)
    {
        storageList << item.storage();
        productList << item.product();
    }

    query.prepare("insert into t_temp_items(f_storage, f_product) "
                  "values (?, ?);");
    query.addBindValue(storageList);
    query.addBindValue(productList);
    db_.beginTransaction();

    if(!query.execBatch())
    {
        db_.rollbackTransaction();
        qInfo() << query.lastError().text();
        qInfo() << query.lastQuery();
        deleteTempItemsTable();
        return false;
    }

    db_.commitTransaction();

    return true;
}

void SalesHistoryStreamReader::deleteTempItemsTable()
{
    QSqlQuery query = db_.getAssociatedQuery();
    db_.beginTransaction();
    query.exec("drop table if exists t_temp_items;");
    db_.commitTransaction();
}

bool SalesHistoryStreamReader::open(const Date &from, const Date &to)
{
    if(items_.isEmpty())
    {
        isCanNext_ = false;
        return false;
    }
    from_ = from;
    to_ = to;

    if(!createTempItemsTable())
    {
        return false;
    }
    loadAnalogsTable();

    QString select = buildSelectString();

    query_ = db_.getAssociatedQuery();
    query_.setForwardOnly(true);

    //----расшифровка плана запроса-----
    //        query_.exec("explain query plan "+ select);
    //        while(query_.next())
    //        {
    //            const QSqlRecord rec = query_.record();
    //            QStringList val;
    //            for(int i = 0; i< rec.count(); i++)
    //            {
    //                val << rec.value(i).toString();
    //            }
    //            qInfo() << val;
    //        }
    //----------------------------------

    //------вывод результатов------
    //    if(query_.exec(select))
    //    {
    //        while(query_.next())
    //        {
    //            QSqlRecord rec = query_.record();
    //            int c = rec.count();
    //            QStringList lst;
    //            for(int i = 0; i < c; i++)
    //            {
    //                lst.append(rec.value(i).toString());
    //            }
    //            qInfo() << lst;

    //        }
    //    }
    //-----------------------------

    if(!query_.exec(select))
    {
        qWarning() << query_.lastError().text();
        qWarning() << query_.lastQuery();
        qWarning() << query_.boundValues();
        return false;
    }

//    if(!query_.lastError().type() == QSqlError::NoError)
//    {
//        return false;
//    }

    if(!query_.next())
    {
        return false;
    }

    isCanNext_ = true;
    const Item item(query_.value(0).toString(),
                    query_.value(1).toString());
    tempHistory_ = SaleHistory(item);
    addDayToTempHistory();

    return true;
}

bool SalesHistoryStreamReader::next()
{
    if(isCanNext_)
    {
        return true;
    }
    else
    {
        deleteTempItemsTable();
        return false;
    }
}

void SalesHistoryStreamReader::addDayToTempHistory()
{
    const Item item(query_.value(0).toString(),
                    query_.value(1).toString());
    const QVariant date = query_.value(2);
    const QVariant sold = query_.value(3);
    const QVariant rest = query_.value(4);
    if(!date.isNull() && !sold.isNull() && !rest.isNull())
    {
        tempHistory_.addDay(SaleHistoryDay(item,
                                           date.toDate(),
                                           sold.toDouble(),
                                           rest.toDouble()));
    }
}

bool SalesHistoryStreamReader::isCanReturnHistory(const Item &tempItemp)
{
    const ID tempStorage = tempHistory_.item().storage();
    const ID storage = tempItemp.storage();

    if(tempHistory_.item() != tempItemp)
    {
        if(tempStorage != storage)
        {
            return true;
        }
        const ID tempProduct = tempHistory_.item().product();
        const ID product = tempItemp.product();
        return analogsTable_.isAnalogical(tempProduct, product).isEmpty();
    }
    return false;
}

void SalesHistoryStreamReader::loadAnalogsTable()
{
    AnalogsReader reader(db_.name());
    QList<ID> idList;
    foreach (const Item &item, items_)
    {
        idList.append(item.product());
    }
    analogsTable_ = reader.read(idList);
}

QString SalesHistoryStreamReader::buildSelectString()
{
    QString select("select t_temp_items.f_storage, "
                   "t_temp_items.f_product, "
                   "t_datas.f_date, "
                   "t_datas.f_sold, "
                   "t_datas.f_rest "
                   "from t_temp_items "
                   "left outer join t_datas "
                   "on t_temp_items.f_storage = t_datas.f_storage "
                   "and t_temp_items.f_product = t_datas.f_product "
                   "%1"
                   "order by t_datas.f_storage, "
                   "t_datas.f_product, "
                   "t_datas.f_date;");

    QString dateCase;
    if(from_ != Date() && to_ != Date())
    {
        dateCase = "where (t_datas.f_date >= '%1' and "
                   "t_datas.f_date <= '%2') "
                   "or t_datas.f_date is null ";
        dateCase = dateCase.arg(from_.toString("yyyy.MM.dd"))
                .arg(to_.toString("yyyy.MM.dd"));
    }
    else if (from_ == Date() && to_ != Date())
    {
        dateCase = "where t_datas.f_date <= '%1' "
                   "or t_datas.f_date is null ";
        dateCase = dateCase.arg(to_.toString("yyyy.MM.dd"));
    }
    else if (to_ == Date() && from_ != Date())
    {
        dateCase = "where t_datas.f_date >= '%1' "
                   "or t_datas.f_date is null ";
        dateCase = dateCase.arg(from_.toString("yyyy.MM.dd"));
    }
    select = select.arg(dateCase);
    return select;
}

void SalesHistoryStreamReader::normalazeTempHistory()
{
    ID mainAnalog;
    if(analogsTable_.isValid())
    {
        mainAnalog =analogsTable_.analogsForProduct(
                    tempHistory_.item().product()).mainAnalog();
    }
    tempHistory_.normalaze(from_, to_, mainAnalog);
}

SaleHistory SalesHistoryStreamReader::current()
{
    while(query_.next())
    {
        const Item tempItemp(query_.value(0).toString(),
                             query_.value(1).toString());
        if(isCanReturnHistory(tempItemp))
        {
            normalazeTempHistory();
            const SaleHistory returnedHistory = tempHistory_;
            tempHistory_ = SaleHistory(tempItemp);
            addDayToTempHistory();
            return returnedHistory;
        }
        addDayToTempHistory();
    }
    isCanNext_ = false;
    normalazeTempHistory();
    return tempHistory_;
}
