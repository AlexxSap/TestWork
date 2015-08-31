#include "DataBaseInfo.h"

DataBaseInfo::Type DataBaseInfo::dataBaseType_ = DataBaseInfo::SQLITE;

DataBaseInfo::DataBaseInfo()
    : dataBaseName_(),
      userName_(),
      userPassword_()
{

}

QString DataBaseInfo::dataBaseName() const
{
    return dataBaseName_;
}

void DataBaseInfo::setDataBaseName(const QString &name)
{
    dataBaseName_ = name;
}

DataBaseInfo::Type DataBaseInfo::dataBaseType()
{
    return dataBaseType_;
}

void DataBaseInfo::setDataBaseType(const DataBaseInfo::Type &type)
{
    dataBaseType_ = type;
}

QString DataBaseInfo::userName() const
{
    return userName_;
}

void DataBaseInfo::setUserName(const QString &name)
{
    userName_ = name;
}

QString DataBaseInfo::password() const
{
    return userPassword_;
}

void DataBaseInfo::setPassword(const QString &password)
{
    userPassword_ = password;
}
