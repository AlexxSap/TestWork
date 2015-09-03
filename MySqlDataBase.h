#ifndef MYSQLDATABASE_H
#define MYSQLDATABASE_H

#include "DataBase.h"

class MySqlDataBase : public DataBase
{
private:
    bool createEmptyDB();
    bool remove();

public:
    MySqlDataBase();
    MySqlDataBase(const DataBaseInfo &info,
                  const QString &connName = "qt_sql_default_connection");
    MySqlDataBase(const DataBase &other);
    ~MySqlDataBase();

    bool isExist();

    bool createTempTableForSalesHistoryStreamReader();
    bool insertValueToTDatas(const QList<QVariantList> &data);
};

#endif // MYSQLDATABASE_H
