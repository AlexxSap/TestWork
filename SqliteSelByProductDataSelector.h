#ifndef SQLITESELBYPRODUCTDATASELECTOR_H
#define SQLITESELBYPRODUCTDATASELECTOR_H

#include "DataBase.h"

class SqliteSelByProductDataSelector : public DataBase
{
private:
    QString caseValue_;

public:
    SqliteSelByProductDataSelector(const QString &dbName);
    ~SqliteSelByProductDataSelector();

    const QSqlQuery get();
    void setCaseValue(const QString &value);
};

#endif // SQLITESELBYPRODUCTDATASELECTOR_H
