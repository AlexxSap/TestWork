#ifndef DATABASEINFO_H
#define DATABASEINFO_H

#include <QString>

class DataBaseInfo
{
public:
    enum Type
    {
        SQLITE
    };

private:
    QString dataBaseName_;
    static Type dataBaseType_;
    QString userName_;
    QString userPassword_;
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
};

#endif // DATABASEINFO_H
