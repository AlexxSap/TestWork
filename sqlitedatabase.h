#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H

#include <QSqlDatabase>
#include <QString>

#include "database.h"

class SqliteDataBase : public DataBase
{
private:
    QString _dbName;

public:
    explicit SqliteDataBase(const QString &dbName);

    bool connect();
    bool disconnect();
    bool isConnected();

    bool beginWrite();
    bool write(const QString &request);
    bool endWrite();

    bool beginRead();
    QSqlQuery read(const QString &request);
    bool endRead();

    QSqlQueryModel* model(const QString &request);
};

#endif // SQLITEDATABASE_H
