#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QString>
#include <QFile>

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
    explicit DataBase(const QString &dbName);
    ~DataBase();

    bool isConnected();

    QSqlQuery getAssociatedQuery() const;

    void beginTransaction();
    void rollbackTransaction();
    void commitTransaction();

};

#endif // DATABASE_H
