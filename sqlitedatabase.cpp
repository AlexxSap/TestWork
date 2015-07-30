#include "SqliteDataBase.h"

SqliteDataBase::SqliteDataBase(const QString &dbName):_dbName(dbName){}


bool SqliteDataBase::connect(){
    return true;
}

bool SqliteDataBase::disconnect(){
    return false;
}

bool SqliteDataBase::isConnected(){
    return false;
}

bool SqliteDataBase::beginWrite(){
    return false;
}

bool SqliteDataBase::write(const QString &request){
    return false;
}
bool SqliteDataBase::endWrite(){
    return false;
}

bool SqliteDataBase::beginRead(){
    return false;
}

QSqlQuery* SqliteDataBase::read(const QString &request){
    return new QSqlQuery();
}

bool SqliteDataBase::endRead(){
    return false;
}

QSqlQueryModel* SqliteDataBase::model(const QString &request){
    return new QSqlQueryModel();
}
