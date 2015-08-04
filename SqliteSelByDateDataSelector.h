#ifndef SQLITESELBYDATEDATASELECTOR_H
#define SQLITESELBYDATEDATASELECTOR_H

#include "DataSelector.h"
#include "SqliteDataBase.h"

class SqliteSelByDateDataSelector : public DataSelector
{
private:
    SqliteDataBase _db;
    QString _caseValue;

public:
    SqliteSelByDateDataSelector(const QString &dbName);
    ~SqliteSelByDateDataSelector();

    QSqlQuery get();
    void setCaseValue(const QString &value);
};

#endif // SQLITESELBYDATEDATASELECTOR_H

