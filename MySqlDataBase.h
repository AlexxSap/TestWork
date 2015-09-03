#ifndef MYSQLDATABASE_H
#define MYSQLDATABASE_H

#include "DataBase.h"

class MySqlDataBase : public DataBase
{
private:
    bool createEmptyDB();
    bool remove();

private:
    bool insertWithManyValues(const QString &beginOfRequest,
                              const QString &valuesRequet,
                              const QList<QVariantList> &data);

public:
    MySqlDataBase();
    MySqlDataBase(const DataBaseInfo &info,
                  const QString &connName = "qt_sql_default_connection");
    MySqlDataBase(const DataBase &other);
    ~MySqlDataBase();

    bool isExist();

    bool insertValuesToTDatas(const QList<SaleHistoryDay> &days);
    bool insertValuesToTAnalogs(const AnalogsTable &table);

    bool createTempTableForSalesHistoryStreamReader();
    bool createTempTableForAnalogsReader();
};

#endif // MYSQLDATABASE_H
