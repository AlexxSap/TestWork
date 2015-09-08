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
      itemsHashTable_(),
      limitSize_(10000),
      limitCounter_(0)
{
    db_->connect();
    itemsHashTable_ = db_->itemsHashTable();
}

SalesHistoryStreamReader::~SalesHistoryStreamReader()
{
    db_->disconnect();
    delete db_;
}

bool SalesHistoryStreamReader::fillInsLists(QVariantList &ids,
                                            QVariantList &main)
{
    const bool isAnalogs = analogsTable_.isValid();

    QList<Item> newItems;
    QStringList newMain;

    foreach (const Item &item, items_)
    {
        const ID storage = item.storage();
        const ID product = item.product();
        const int id = itemsHashTable_.key(item, -1);
        if(id != -1)
        {
            ids << id;
        }
        else
        {
            newItems << item;
        }

        if(isAnalogs)
        {
            const Analogs analogs =analogsTable_.analogsForProduct(product);
            const ID mainAn = analogs.mainAnalog();
            if(id != -1)
            {
                main << mainAn;
            }
            else
            {
                newMain << mainAn;
            }

            const QList<ID> analogsIDs = analogs.toList();
            foreach (const ID &temp, analogsIDs)
            {
                const int tempId = itemsHashTable_.key(Item(storage, temp), -1);;
                if(tempId != -1)
                {
                    ids << tempId;
                    main << mainAn;
                }
            }
        }
        else
        {
            main << QVariant("");
            newMain << QString();
        }
    }

    if(!newItems.isEmpty())
    {
        QHash<int, Item> newHashTableItems  = insertNewItemsToTItems(newItems);
        if(newHashTableItems.isEmpty())
        {
            return false;
        }
        QList<int> keys = newHashTableItems.keys();

        for(int i = 0; i < keys.count(); i++)
        {
            ids << keys.at(i);
            main << newMain.at(i);
        }
    }
    return true;
}

QHash<int, Item> SalesHistoryStreamReader::insertNewItemsToTItems(const QList<Item> &newItems)
{

    QHash<int, Item> newItemsTable;

    QList<int> keys = itemsHashTable_.keys();
    qSort(keys);
    int max = keys.last() + 1;

    foreach (const Item &item, newItems)
    {
        newItemsTable.insert(max, item);
        max++;
    }
    if(db_->insertToTItems(newItemsTable))
    {
        return newItemsTable;
    }

    return QHash<int, Item>();
}

bool SalesHistoryStreamReader::fillTempItemsTable()
{
    QVariantList idList;
    QVariantList mainAnList;

    if(!fillInsLists(idList, mainAnList))
    {
        return false;
    }

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

    if(!query.exec("insert into tTempOrder(fItem, fStorage, fProduct,  fMainAn) "
                   "select distinct tTempItems.fItem, tItems.fStorage , tItems.fProduct , tTempItems.fMainAn "
                   "from tTempItems "
                   "left join tItems "
                   "on tItems.fItem = tTempItems.fItem "
                   "order by tItems.fStorage, tTempItems.fMainAn, "
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
    itemsHashTable_.clear();
    items_.clear();

    QString select = db_->selectForSalesHistoryStreamReader(from_, to_);

    query_ = db_->associatedQuery();
    query_.setForwardOnly(true);

//    select = "select fItem, fDate, fSold, fRest from tDatas;";


//    const double sPrep = Utils::_runBenchmarking("prepare");
    query_.prepare(select);
//    Utils::_endBenchmarking("prepare", sPrep);

//    const double sExec = Utils::_runBenchmarking("exec");

    if(!query_.exec())
    {
        qWarning() << query_.lastError().text();
        return false;
    }
//    Utils::_endBenchmarking("exec", sExec);

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
