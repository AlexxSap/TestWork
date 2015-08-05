#ifndef SQLITESELBYDATEDATASELECTOR_H
#define SQLITESELBYDATEDATASELECTOR_H

#include "DataBase.h"

class SqliteSelByDateDataSelector : public DataBase
{
private:
    QString caseValue_;

public:
    SqliteSelByDateDataSelector(const QString &dbName);
    ~SqliteSelByDateDataSelector();

    const QSqlQuery get();
    void setCaseValue(const QString &value);
};

#endif // SQLITESELBYDATEDATASELECTOR_H

