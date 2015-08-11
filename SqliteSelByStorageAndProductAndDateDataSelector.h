#ifndef SQLITESELBYSTORAGEANDPRODUCTANDDATEDATASELECTOR_H
#define SQLITESELBYSTORAGEANDPRODUCTANDDATEDATASELECTOR_H

#include <QDate>
#include <QDebug>
#include <QSqlError>
#include "DataBase.h"

class SqliteSelByStorageAndProductAndDateDataSelector : public DataBase
{
private:
    QString product_;
    QString storage_;
    QDate fromDate_;
    QDate toDate_;

public:
    SqliteSelByStorageAndProductAndDateDataSelector(const QString &dbName);
    const QSqlQuery get();
    void setCaseProduct(const QString &product);
    void setCaseStorage(const QString &storage);
    void setCaseDateInterval(const QDate &fromDate, const QDate &toDate);
};

#endif // SQLITESELBYSTORAGEANDPRODUCTANDDATEDATASELECTOR_H
