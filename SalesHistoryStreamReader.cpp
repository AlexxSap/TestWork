#include "SalesHistoryStreamReader.h"

SalesHistoryStreamReader::SalesHistoryStreamReader(const QList<Item> &items,
                                                   const DataBaseInfo &info)
    :items_(items),
      db_(DataBase::getDataBase(info)),
      query_(),
      from_(),
      to_(),
      tempHistory_(),
      isCanNext_(false),
      analogsTable_(),
      itemsHashTable_()
{
    db_->connect();
    itemsHashTable_ = db_->itemsHashTable();
}

SalesHistoryStreamReader::~SalesHistoryStreamReader()
{
    db_->disconnect();
    delete db_;
}

void SalesHistoryStreamReader::fillInsLists(QVariantList &ids,
                                            QVariantList &main) const
{
    const bool isAnalogs = analogsTable_.isValid();

    foreach (const Item &item, items_)
    {
        const ID storage = item.storage();
        const ID product = item.product();
        const int id = itemsHashTable_.key(Item(storage, product), -1);
        if(id != -1)    // если такого товара нет, то и искать его мы не будем...
        {
            ids << id;
        }

        if(isAnalogs)
        {
            const Analogs analogs =analogsTable_.analogsForProduct(product);
            const ID mainAn = analogs.mainAnalog();
            if(id != -1)
            {
                main << mainAn;
            }
            // ... но аналоги посмотрим...
            const QList<ID> analogsIDs = analogs.toList();
            foreach (const ID &temp, analogsIDs)
            {
                const int tempId = itemsHashTable_.key(Item(storage, temp), -1);;
                if(tempId != -1)    //... те которые есть
                {
                    ids << tempId;
                    main << mainAn;
                }
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
    QVariantList idList;
    QVariantList mainAnList;

    fillInsLists(idList, mainAnList);

    QSqlQuery query = db_->associatedQuery();
    query.prepare("insert into tTempItems(fItem, fMainAn) "
                  "values (?, ?);");
    query.addBindValue(idList);
    query.addBindValue(mainAnList);

    db_->beginTransaction();
    if(!query.execBatch())
    {
        db_->rollbackTransaction();
        qInfo() << query.lastError().text();
        db_->dropTempTableForSalesHistoryStreamReader();
        return false;
    }

    if(!query.exec("insert into tTempOrder(fItem, fMainAn) "
                  "select distinct fItem, fMainAn "
                   "from tTempItems "
                   "order by  tTempItems.fMainAn, "
                   "tTempItems.fItem;"))
    {
        db_->rollbackTransaction();
        qInfo() << query.lastError().text();
        qInfo() << query.lastQuery();
        db_->dropTempTableForSalesHistoryStreamReader();
        return false;
    }
    db_->dropTempTableItemsForSalesHistoryStreamReader();
    db_->commitTransaction();
    return true;
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

    fetchAnalogsTable();

    if(!db_->createTempTableForSalesHistoryStreamReader())
    {
        return false;
    }

    if(!fillTempItemsTable())
    {
        return false;
    }

    query_ = db_->queryForSalesHistoryStreamReader(from_, to_);

    if(!query_.exec())
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
        db_->dropTempTableForSalesHistoryStreamReader();
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

void SalesHistoryStreamReader::fetchAnalogsTable()
{
    AnalogsReader reader(db_->info());
    QList<ID> idList;
    foreach (const Item &item, items_)
    {
        const ID product = item.product();
        if(!idList.contains(product))
        {
            idList.append(product);
        }
    }
    analogsTable_ = reader.fetch(idList);
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
