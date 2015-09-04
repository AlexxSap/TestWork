#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QFile>
#include <QSqlRecord>
#include <QPointer>
#include <QDate>
#include <QDebug>

#include "DataBaseInfo.h"
#include "AnalogsTable.h"
#include "SaleHistoryDay.h"

class DataBase : public QObject
{
protected:
    DataBaseInfo info_;
    QSqlDatabase db_;
    QString connectionName_;

private:
    virtual bool createEmptyDB() = 0;
    virtual bool remove() = 0;

protected:
    bool executeQuery(QSqlDatabase &db, const QString &request);

public:
    DataBase();
    explicit DataBase(const DataBaseInfo &info,
             const QString &connName = "def_conn");
    DataBase(const DataBase &other);
    virtual ~DataBase();

    const DataBaseInfo info() const;
    const QString connectionName() const;

    void beginTransaction();
    void rollbackTransaction();
    void commitTransaction();

    bool connect();
    bool disconnect();
    bool isConnected();

    const QHash<int, Item>& itemsHashTable();

    QSqlQuery associatedQuery() const;

    virtual bool isExist() = 0;
    virtual bool insertValuesToTDatas(const QList<SaleHistoryDay> &days,
                                      const QHash<int, Item> &items,
                                      const QHash<int, Item> &newItems) = 0;
    virtual bool insertValuesToTAnalogs(const AnalogsTable &table) = 0;

    virtual bool createTempTableForAnalogsReader() = 0;
    virtual void dropTempTableForAnalogsReader();

    virtual bool createTempTableForSalesHistoryStreamReader() = 0;
    virtual void dropTempTableForSalesHistoryStreamReader();
    virtual void dropTempTableItemsForSalesHistoryStreamReader();

    virtual QSqlQuery queryForAnalogsReader(const bool &forward = true);
    virtual QSqlQuery queryForSalesHistoryStreamReader(const QDate &from,
                                                       const QDate &to,
                                                       const bool &forward = true) = 0;

    static bool remDataBase(const DataBaseInfo &info);
    static QPointer<DataBase> getDataBase(const DataBaseInfo &info,
                                          const QString &connName = "def_conn");
};

#endif // DATABASE_H
