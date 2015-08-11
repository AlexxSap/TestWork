#ifndef SQLITESELBYSTORAGEANDPRODUCTDATASELECTOR_H
#define SQLITESELBYSTORAGEANDPRODUCTDATASELECTOR_H

#include "DataBase.h"

class SqliteSelByStorageAndProductDataSelector : public DataBase
{
private:
    QString product_;
    QString storage_;

public:
    SqliteSelByStorageAndProductDataSelector(const QString &dbName);
    const QSqlQuery get();
    void setCaseProduct(const QString &product);
    void setCaseStorage(const QString &storage);
};

#endif // SQLITESELBYSTORAGEANDPRODUCTDATASELECTOR_H
