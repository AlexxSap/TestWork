#include "DataBaseInfo.h"

DataBaseInfo::Type DataBaseInfo::dataBaseType_ = DataBaseInfo::SQLITE;

DataBaseInfo::DataBaseInfo()
    : dataBaseName_(),
      userName_("testUser"),
      userPassword_("testPassword"),
      hostName_("localhost")
{

}

QString DataBaseInfo::dataBaseName() const
{
    QString name = dataBaseName_;
    if(dataBaseType_ == SQLITE)
    {
        name += ".db";
    }
    return name;
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

QString DataBaseInfo::hostName() const
{
    return hostName_;
}

void DataBaseInfo::setHostName(const QString &host)
{
    hostName_ = host;
}
