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
#include "database.h"
#include "SqliteAllDataDataSelector.h"
#include "SqliteSelByProductDataSelector.h"
#include "SqliteSelByDateDataSelector.h"

class TestWriteReadFileToDB : public QObject
{
    Q_OBJECT
public:
    explicit TestWriteReadFileToDB(QObject *parent = 0);

private:
    void createFile(const QString &fileName, const QStringList &data);
    void removeFile(const QString &fileName);
    void createTestDB(const QString &dbName);
    QStringList readDataFromFile(const QString &fileName);
    bool createTestDbStructure(QSqlDatabase &db);

private:
    bool compData(const QStringList &act, const QStringList &exp);
    bool compError(const int &act, const int &exp);

private slots:
    void testWriteReadFileToDB();
    void testWriteReadFileToDB_data();
};

#endif // TESTWRITEREADFILETODB_H
