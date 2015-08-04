#ifndef SQLITESELBYPRODUCTDATASELECTOR_H
#define SQLITESELBYPRODUCTDATASELECTOR_H

#include "DataSelector.h"
#include "SqliteDataBase.h"

class SqliteSelByProductDataSelector : public DataSelector
{
private:
    ///notes у нас принято private поля обозначать суффиксом _, а не префиксом
    SqliteDataBase _db;
    QString _caseValue;

public:
    SqliteSelByProductDataSelector(const QString &dbName);
    ~SqliteSelByProductDataSelector();

    QSqlQuery get();
    void setCaseValue(const QString &value);
};

#endif // SQLITESELBYPRODUCTDATASELECTOR_H
