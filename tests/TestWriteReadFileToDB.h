#ifndef TESTWRITEREADFILETODB_H
#define TESTWRITEREADFILETODB_H

#include <QObject>
#include <QTest>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>
#include <QString>

#include "FileReader.h"
#include "CsvFileReader.h"
#include "CsvFileWriter.h"
#include "sqlitedatabase.h"

/*
Пока не реализована структура хранения данных в памяти
    тестирование идёт по считыванию всех данных из БД
    и записи в файл.
*/

class TestWriteReadFileToDB : public QObject
{
    Q_OBJECT
public:
    explicit TestWriteReadFileToDB(QObject *parent = 0);

private:
    void createFile(const QStringList &data);
    void removeFiles();
    bool createTestDB();
    QStringList readDataFromFile(const QString &fileName);

private:
    QString _testInFileName;
    QString _testOutFileName;
    QString _testDBName;

    bool compData(const QStringList &act, const QStringList &exp);
    bool compError(const int &act, const int &exp);

private slots:
    void writeReadFileToDB();
    void writeReadFileToDB_data();
};

#endif // TESTWRITEREADFILETODB_H
