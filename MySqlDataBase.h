#ifndef MYSQLDATABASE_H
#define MYSQLDATABASE_H

#include "DataBase.h"

class MySqlDataBase : public DataBase
{
private:
    bool createEmptyDB();
    bool remove();


private:
    void init(const QString &connName);
    bool insertWithManyValues(const QString &tableDescr,
                              const QList<QVariantList> &data);

public:
    MySqlDataBase();
    explicit MySqlDataBase(const DataBaseInfo &info,
                           const QString &connName = "defConn");
    MySqlDataBase(const DataBase &other);
    ~MySqlDataBase();

    bool isExist();
    bool insertToTItems(const QHash<int, Item> &newItems);

    bool insertValuesToTDatas(const QList<SaleHistoryDay> &days,
                              const QHash<int, Item> &items,
                              const QHash<int, Item> &newItems);
    bool insertValuesToTAnalogs(const AnalogsTable &table);

    bool createTempTableForSalesHistoryStreamReader();
    bool createTempTableForAnalogsReader();

    QString selectForSalesHistoryStreamReader(const QDate &from,
                                              const QDate &to);
};

#endif // MYSQLDATABASE_H
