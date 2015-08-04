#ifndef TESTFILEREDER_H
#define TESTFILEREDER_H

#include <QObject>
#include <QTest>
#include <QStringList>
#include <QListView>

#include "FileReader.h"
#include "CsvFileReader.h"
#include "database.h"

//нужен для вызова CsvFileReader::readFromFile
class DBForTestFileReader: public DataBase
{
public:
    DBForTestFileReader(){}
    ~DBForTestFileReader(){}

    bool connect(){return true;}
    bool disconnect(){return true;}
    bool isConnected(){return true;}

    bool write(const QString &request){Q_UNUSED(request); return true;}
    QSqlQuery read(const QString &request){Q_UNUSED(request); return QSqlQuery();}

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
    void testLoadFile();
    void testLoadFile_data();
};

#endif // TESTFILEREDER_H
