#ifndef DATABASEINFO_H
#define DATABASEINFO_H

#include "DataBase.h"

class DataBaseInfo
{
private:
    static QString dataBaseName_;
    static DataBase::Type dataBaseType_;
    static QString userName_;
    static QString userPassword_;
    static QString connectionName_;

public:
    static QString dataBaseName();
    static void setDataBaseName(const QString &name);

    static DataBase::Type dataBaseType();
    static void setDataBaseType(const DataBase::Type &type);

    static QString userName();
    static void setUserName(const QString &name);

    static QString password();
    static void setPassword(const QString &password);

    static QString connectionName();
    static void setConnectionName(const QString &name);

};

#endif // DATABASEINFO_H
