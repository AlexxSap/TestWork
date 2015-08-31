#include "DataBaseInfo.h"

QString DataBaseInfo::dataBaseName_ = "";
DataBase::Type DataBaseInfo::dataBaseType_ = DataBase::SQLITE;
QString DataBaseInfo::userName_ = "";
QString DataBaseInfo::userPassword_ = "";
QString DataBaseInfo::connectionName_ = "qt_sql_default_connection";

QString DataBaseInfo::dataBaseName()
{
    return dataBaseName_;
}

void DataBaseInfo::setDataBaseName(const QString &name)
{
    dataBaseName_ = name;
}

DataBase::Type DataBaseInfo::dataBaseType()
{
    return dataBaseType_;
}

void DataBaseInfo::setDataBaseType(const DataBase::Type &type)
{
    dataBaseType_ = type;
}

QString DataBaseInfo::userName()
{
    return userName_;
}

void DataBaseInfo::setUserName(const QString &name)
{
    userName_ = name;
}

QString DataBaseInfo::password()
{
    return userPassword_;
}

void DataBaseInfo::setPassword(const QString &password)
{
    userPassword_ = password;
}

QString DataBaseInfo::connectionName()
{
    return connectionName_;
}

void DataBaseInfo::setConnectionName(const QString &name)
{
    connectionName_ = name;
}

