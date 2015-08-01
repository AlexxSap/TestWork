#ifndef SQLITEALLDATADATASELECTOR_H
#define SQLITEALLDATADATASELECTOR_H

#include "DataSelector.h"
#include "SqliteDataBase.h"


class SqliteAllDataDataSelector : public DataSelector
{
private:
    SqliteDataBase _db;

public:
    SqliteAllDataDataSelector(const QString &dbName);
    ~SqliteAllDataDataSelector();

    QSqlQuery get();
};

#endif // SQLITEALLDATADATASELECTOR_H
