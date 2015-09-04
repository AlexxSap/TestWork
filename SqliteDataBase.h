#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H

#include "DataBase.h"


class SqliteDataBase : public DataBase
{
private:
    bool createEmptyDB();
    bool remove();

private:
    void init(const QString &connName);

public:
    SqliteDataBase();
    explicit SqliteDataBase(const DataBaseInfo &info,
                   const QString &connName = "qt_sql_default_connection");
    SqliteDataBase(const DataBase &other);
    ~SqliteDataBase();

    bool isExist();
    bool insertToTItems(const QHash<int, Item> &newItems);

    bool insertValuesToTDatas(const QList<SaleHistoryDay> &days,
                              const QHash<int, Item> &items,
                              const QHash<int, Item> &newItems);
    bool insertValuesToTAnalogs(const AnalogsTable &table);

    bool createTempTableForAnalogsReader();
    bool createTempTableForSalesHistoryStreamReader();

    QString selectForSalesHistoryStreamReader(const QDate &from,
                                                         const QDate &to,
                                                         const bool &forward);

};

#endif // SQLITEDATABASE_H
