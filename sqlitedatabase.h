#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H

#include <QSqlDatabase>
#include <QString>

#include "database.h"

class SqliteDataBase : public DataBase
{
private:
    QString _dbName;
    QSqlDatabase _db;

public:
    SqliteDataBase(const QString &dbName);
    ~SqliteDataBase();

    bool connect();
    bool disconnect();
    bool isConnected();

    bool write(const QString &request);
    QSqlQuery read(const QString &request);

    QSqlQueryModel* model(const QString &request);
};

#endif // SQLITEDATABASE_H
