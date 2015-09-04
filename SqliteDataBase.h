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
    bool insertToTItems(const QHash<int, Item> &newItems);

public:
    SqliteDataBase();
    explicit SqliteDataBase(const DataBaseInfo &info,
                   const QString &connName = "qt_sql_default_connection");
    SqliteDataBase(const DataBase &other);
    ~SqliteDataBase();

    bool isExist();

    bool insertValuesToTDatas(const QList<SaleHistoryDay> &days,
                              const QHash<int, Item> &items,
                              const QHash<int, Item> &newItems);
    bool insertValuesToTAnalogs(const AnalogsTable &table);

    bool createTempTableForAnalogsReader();
    bool createTempTableForSalesHistoryStreamReader();

    QSqlQuery queryForSalesHistoryStreamReader(const QDate &from,
                                                         const QDate &to,
                                                         const bool &forward);

};

#endif // SQLITEDATABASE_H
