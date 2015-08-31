#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H

#include "DataBase.h"

class SqliteDataBase : public DataBase
{
private:
    bool createEmptyDB();

public:
    SqliteDataBase();
    SqliteDataBase(const QString &dbName,
             const QString &connName = "qt_sql_default_connection");
    SqliteDataBase(const DataBase &other);

    ~SqliteDataBase();

};

#endif // SQLITEDATABASE_H
