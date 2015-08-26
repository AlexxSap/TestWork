#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QFile>

#include <QDebug>

class DataBase
{
private:
    QString dbName_;
    QSqlDatabase db_;

private:
    bool createEmptyDB();
    bool executeQuery(QSqlDatabase &db, const QString &request);
    bool connect();
    bool disconnect();
    void setPragmaParameters(QSqlDatabase &db);

public:
    explicit DataBase(const QString &dbName, const QString &connName = "qt_sql_default_connection");

    ~DataBase();

    bool isConnected();

    QSqlQuery getAssociatedQuery() const;
    const QString name() const;

    void beginTransaction();
    void rollbackTransaction();
    void commitTransaction();

    bool createTempTableForWrite();
    bool dropTempTableForWrite();
};

#endif // DATABASE_H
