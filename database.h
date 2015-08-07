#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QString>

class DataBase
{
private:
    QString dbName_;
    QSqlDatabase db_;

protected:
    bool connect();
    bool disconnect();

public:
    explicit DataBase(const QString &dbName);
    ~DataBase();

    bool isConnected();

    const virtual QSqlQuery get();
    //const QSqlDatabase getDB() const;
    QSqlQuery getAssociatedQuery() const;

    void beginTransaction();
    void rollbackTransaction();
    void commitTransaction();
};

#endif // DATABASE_H
