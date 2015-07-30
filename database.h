#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlQuery>
#include <QSqlQueryModel>

class DataBase
{
public:
    ///notes все реализации методов и конструкторов должны быть в cpp файле
    DataBase(){}
    virtual ~DataBase(){}

    virtual bool connect()=0;
    virtual bool disconnect()=0;
    virtual bool isConnected()=0;

    virtual bool beginWrite()=0;
    virtual bool write(const QString &request)=0;
    virtual bool endWrite()=0;

    virtual bool beginRead()=0;


    ///notes не понимаю необходимость возвращать указатель
    virtual QSqlQuery* read(const QString &request="")=0;
    virtual bool endRead()=0;

    virtual QSqlQueryModel* model(const QString &request="")=0;
};

#endif // DATABASE_H
