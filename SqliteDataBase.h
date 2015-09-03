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
    SqliteDataBase(const DataBaseInfo &info,
                   const QString &connName = "qt_sql_default_connection");
    SqliteDataBase(const DataBase &other);
    ~SqliteDataBase();

    bool isExist();

    bool insertValuesToTDatas(const QList<SaleHistoryDay> &days);
    bool insertValuesToTAnalogs(const AnalogsTable &table);

    bool createTempTableForAnalogsReader();
    bool createTempTableForSalesHistoryStreamReader();
};

#endif // SQLITEDATABASE_H
