#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H

#include "DataBase.h"

class SqliteDataBase : public DataBase
{
private:
    bool createEmptyDB();
    bool remove();

public:
    SqliteDataBase();
    SqliteDataBase(const DataBaseInfo &info,
                   const QString &connName = "qt_sql_default_connection");
    SqliteDataBase(const DataBase &other);
    ~SqliteDataBase();

    bool isExist();

    bool insertValueToTDatas(const QList<QVariantList> &data);
};

#endif // SQLITEDATABASE_H
