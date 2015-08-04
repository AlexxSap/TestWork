#ifndef SQLITESELBYPRODUCTDATASELECTOR_H
#define SQLITESELBYPRODUCTDATASELECTOR_H

#include "DataSelector.h"
#include "SqliteDataBase.h"

class SqliteSelByProductDataSelector : public DataSelector
{
private:
    SqliteDataBase _db;
    QString _caseValue;

public:
    SqliteSelByProductDataSelector(const QString &dbName);
    ~SqliteSelByProductDataSelector();

    QSqlQuery get();
    void setCaseValue(const QString &value);
};

#endif // SQLITESELBYPRODUCTDATASELECTOR_H
