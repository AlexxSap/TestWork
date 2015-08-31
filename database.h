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

#include <QDebug>

class DataBase : public QObject
{
public:
    enum Type
    {
        SQLITE
    };

private:
    QString dbName_;
    QSqlDatabase db_;
    Type type_;

private:
    virtual bool createEmptyDB() = 0;

protected:
    bool executeQuery(QSqlDatabase &db, const QString &request);

public:
    DataBase();
    DataBase(const QString &dbName,
             const QString &connName = "qt_sql_default_connection");
    DataBase(const DataBase &other);
    virtual ~DataBase();

    const QString name() const;
    Type type();

    void beginTransaction();
    void rollbackTransaction();
    void commitTransaction();

    bool connect();
    bool disconnect();
    bool isConnected();

    QSqlQuery getAssociatedQuery() const;

    virtual bool createTempTableForAnalogsReader();
    virtual void dropTempTableForAnalogsReader();

    virtual bool createTempTableForSalesHistoryStreamReader();
    virtual void dropTempTableForSalesHistoryStreamReader();
};

QPointer<DataBase> getDataBase(const QString &dbName,
                     const DataBase::Type &type = DataBase::SQLITE,
                     const QString &connName = "qt_sql_default_connection");

#endif // DATABASE_H

/*
отличия sqlite от mysql
тип данных для даты
QSqlDatabase::addDatabase
setPragmaParameters
*/
