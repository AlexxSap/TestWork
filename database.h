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
    DataBase(const DataBaseInfo &info,
             const QString &connName = "qt_sql_default_connection");
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

    virtual bool isExist() = 0;

    QSqlQuery associatedQuery() const;

    virtual bool insertValueToTDatas(const QList<QVariantList> &data) = 0;

    virtual bool createTempTableForAnalogsReader();
    virtual void dropTempTableForAnalogsReader();

    virtual bool createTempTableForSalesHistoryStreamReader();
    virtual void dropTempTableForSalesHistoryStreamReader();

    virtual QSqlQuery queryForAnalogsReader(const bool &forward = true);
    virtual QSqlQuery queryForSalesHistoryStreamReader(const QDate &from,
                                                       const QDate &to,
                                                       const bool &forward = true);

    static bool remDataBase(const DataBaseInfo &info);
    static QPointer<DataBase> getDataBase(const DataBaseInfo &info,
                                          const QString &connName = "qt_sql_default_connection");
};

#endif // DATABASE_H
