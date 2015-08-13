#ifndef TESTUTILITY_H
#define TESTUTILITY_H

#include <QSqlDatabase>
#include <QFile>
#include <QSqlQuery>

class TestUtility
{
private:
    static bool createTestDbStructure(QSqlDatabase &db);

public:
    static bool createFile(const QString &fileName, const QStringList &data);
    static bool removeFile(const QString &fileName);
    static bool createTestDB(const QString &dbName);
    static bool compareListWithoutOrder(const QStringList &act, const QStringList &exp);

};

#endif // TESTUTILITY_H
