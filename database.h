#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QFile>
#include <QSqlRecord>

#include <QDebug>

class DataBase
{
private:
    QString dbName_;
    QSqlDatabase db_;


private:
    virtual bool createEmptyDB();
    bool executeQuery(QSqlDatabase &db, const QString &request);
    bool connect();
    bool disconnect();
    void setPragmaParameters(QSqlDatabase &db);

public:
    DataBase(const QString &dbName,
             const QString &connName = "qt_sql_default_connection");

    ~DataBase();

    bool isConnected();

    QSqlQuery getAssociatedQuery() const;
    const QString name() const;

    void beginTransaction();
    void rollbackTransaction();
    void commitTransaction();

    virtual bool createTempTableForAnalogsReader();
    virtual void dropTempTableForAnalogsReader();

    virtual bool createTempTableForSalesHistoryStreamReader();
    virtual void dropTempTableForSalesHistoryStreamReader();
};

#endif // DATABASE_H

/*
отличия sqlite от mysql
тип данных для даты
QSqlDatabase::addDatabase
setPragmaParameters

*/
