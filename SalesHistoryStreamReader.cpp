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

void SalesHistoryStreamReader::fillInsLists(QVariantList &stor,
                                            QVariantList &prod,
                                            QVariantList &main) const
{
    const bool isAnalogs = analogsTable_.isValid();

    foreach (const Item &item, items_)
    {
        const ID product = item.product();
        const ID storage = item.storage();
        stor << storage;
        prod << product;

        if(isAnalogs)
        {
            const Analogs analogs =analogsTable_.analogsForProduct(product);
            const ID mainAn = analogs.mainAnalog();
            main << mainAn;

            const QList<ID> analogsIDs = analogs.toList();
            foreach (const ID temp, analogsIDs)
            {
                stor << storage;
                prod << temp;
                main << mainAn;
            }
        }
        else
        {
            main << QVariant();
        }
    }
}

bool SalesHistoryStreamReader::fillTempItemsTable()
{
    QVariantList storageList;
    QVariantList productList;
    QVariantList mainAnList;

    fillInsLists(storageList, productList, mainAnList);

    QSqlQuery query = db_.getAssociatedQuery();
    query.prepare("insert into t_temp_items(f_storage, f_product, f_main_an) "
                  "values (?, ?, ?);");
    query.addBindValue(storageList);
    query.addBindValue(productList);
    query.addBindValue(mainAnList);

    db_.beginTransaction();
    if(!query.execBatch())
    {
        db_.rollbackTransaction();
        qInfo() << query.lastError().text();
        qInfo() << query.lastQuery();
        db_.dropTempTableForSalesHistoryStreamReader();
        return false;
    }

    if(!query.exec("insert into t_temp_order(f_storage, f_product, f_main_an) "
                  "select distinct f_storage, f_product, f_main_an "
                   "from t_temp_items "
                   "order by  t_temp_items.f_main_an, "
                   "t_temp_items.f_storage, "
                   "t_temp_items.f_product;"))
    {
        db_.rollbackTransaction();
        qInfo() << query.lastError().text();
        qInfo() << query.lastQuery();
        db_.dropTempTableForSalesHistoryStreamReader();
        return false;
    }
    //------вывод результатов------
//    if(query_.exec("select * from t_temp_order;"))
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

    db_.commitTransaction();
    return true;
}

QString SalesHistoryStreamReader::buildSelectString() const
{
    QString select("select t_temp_order.f_storage, "
                   "t_temp_order.f_product, "
                   "t_datas.f_date, "
                   "t_datas.f_sold, "
                   "t_datas.f_rest "
                   "from t_temp_order "
                   "left outer join t_datas "
                   "on t_temp_order.f_storage = t_datas.f_storage "
                   "and t_temp_order.f_product = t_datas.f_product "
                   "%1"
                   "order by t_temp_order.f_order;");

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

bool SalesHistoryStreamReader::open(const Date &from, const Date &to)
{
    if(items_.isEmpty())
    {
        isCanNext_ = false;
        return false;
    }
    from_ = from;
    to_ = to;

    loadAnalogsTable();

    if(!db_.createTempTableForSalesHistoryStreamReader())
    {
        return false;
    }

    if(!fillTempItemsTable())
    {
        return false;
    }

    QString select = buildSelectString();

    query_ = db_.getAssociatedQuery();
    query_.setForwardOnly(true);

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

    if(!query_.next())
    {
        return false;
    }

    isCanNext_ = true;
    tempHistory_ = SaleHistory(Item(query_.value(0).toString(),
                                    query_.value(1).toString()));
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
        db_.dropTempTableForSalesHistoryStreamReader();
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

bool SalesHistoryStreamReader::isCanReturnHistory(const Item &item) const
{
    const Item tempItem = tempHistory_.item();
    if(tempItem != item)
    {
        if(tempItem.storage() != item.storage())
        {
            return true;
        }
        return analogsTable_.isAnalogical(tempItem.product(),
                                          item.product()).isEmpty();
    }
    return false;
}

void SalesHistoryStreamReader::loadAnalogsTable()
{
    AnalogsReader reader(db_.name());
    QList<ID> idList;
    foreach (const Item &item, items_)
    {
        const ID product = item.product();
        if(!idList.contains(product))
        {
            idList.append(product);
        }
    }
    analogsTable_ = reader.read(idList);
}

void SalesHistoryStreamReader::normalazeTempHistory()
{
    ID mainAnalog;
    if(analogsTable_.isValid())
    {
        mainAnalog = analogsTable_.analogsForProduct(
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
