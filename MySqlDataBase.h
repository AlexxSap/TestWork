#ifndef MYSQLDATABASE_H
#define MYSQLDATABASE_H

#include "DataBase.h"

class MySqlDataBase : public DataBase
{
private:
    bool createEmptyDB();

public:
    MySqlDataBase();
    MySqlDataBase(const DataBaseInfo &info,
                  const QString &connName = "qt_sql_default_connection");
    MySqlDataBase(const DataBase &other);

    ~MySqlDataBase();

    bool createTempTableForSalesHistoryStreamReader();
};

#endif // MYSQLDATABASE_H
