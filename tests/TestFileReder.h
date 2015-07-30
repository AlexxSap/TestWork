#ifndef TESTFILEREDER_H
#define TESTFILEREDER_H

#include <QObject>
#include <QTest>
#include <QStringList>
#include <QListView>

#include "FileReader.h"
#include "CsvFileReader.h"
#include "database.h"

///notes опечатка в названии класса

///notes не вижу в тестах мотивации для создания иерархии классов и моков, ну да ладно.
class DBForTestFileReder: public DataBase{
public:
    DBForTestFileReder(){}
    ~DBForTestFileReder(){}

    bool connect(){return true;}
    bool disconnect(){return true;}
    bool isConnected(){return true;}

    bool beginWrite(){return true;}
    bool write(const QString &request){Q_UNUSED(request); return true;}
    bool endWrite(){return true;}

    bool beginRead(){return true;}
    QSqlQuery* read(const QString &request){Q_UNUSED(request); return new QSqlQuery();}
    bool endRead(){return true;}

    QSqlQueryModel* model(const QString &request){Q_UNUSED(request); return new QSqlQueryModel();}
};

class TestFileReder : public QObject
{
    Q_OBJECT
public:
    explicit TestFileReder(QObject *parent = 0);
private:
    void createFile(const QString &name, const QStringList &data);
    void removeFile(const QString &name);
private slots:

    ///notes тесты у нас принято называть с префикса Test, например, TestLoadFile
    void loadFile();
    void loadFile_data();
};

#endif // TESTFILEREDER_H
