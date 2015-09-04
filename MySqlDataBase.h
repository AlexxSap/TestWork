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
    bool insertToTItems(const QHash<int, Item> &newItems);

public:
    MySqlDataBase();
    explicit MySqlDataBase(const DataBaseInfo &info,
                           const QString &connName = "qt_sql_default_connection");
    MySqlDataBase(const DataBase &other);
    ~MySqlDataBase();

    bool isExist();

    bool insertValuesToTDatas(const QList<SaleHistoryDay> &days,
                              const QHash<int, Item> &items,
                              const QHash<int, Item> &newItems);
    bool insertValuesToTAnalogs(const AnalogsTable &table);

    bool createTempTableForSalesHistoryStreamReader();
    bool createTempTableForAnalogsReader();

    QSqlQuery queryForSalesHistoryStreamReader(const QDate &from,
                                                         const QDate &to,
                                                         const bool &forward);
};

#endif // MYSQLDATABASE_H
