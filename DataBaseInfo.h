#ifndef DATABASEINFO_H
#define DATABASEINFO_H

#include <QString>

class DataBaseInfo
{
public:
    enum Type
    {
        SQLITE,
        MYSQL
    };

private:
    QString dataBaseName_;
    static Type dataBaseType_;
    QString userName_;
    QString userPassword_;
    QString hostName_;

public:
    DataBaseInfo();

    QString dataBaseName() const;
    void setDataBaseName(const QString &name);

    static Type dataBaseType();
    static void setDataBaseType(const Type &type);

    QString userName() const;
    void setUserName(const QString &name);

    QString password() const;
    void setPassword(const QString &password);

    QString hostName() const;
    void setHostName(const QString &host);
};

#endif // DATABASEINFO_H
