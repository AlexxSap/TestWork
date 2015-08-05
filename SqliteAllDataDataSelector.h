#ifndef SQLITEALLDATADATASELECTOR_H
#define SQLITEALLDATADATASELECTOR_H

#include "DataBase.h"

class SqliteAllDataDataSelector : public DataBase
{
public:
    SqliteAllDataDataSelector(const QString &dbName);
    ~SqliteAllDataDataSelector();

    const QSqlQuery get();
};

#endif // SQLITEALLDATADATASELECTOR_H
