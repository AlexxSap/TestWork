#ifndef TESTSALEHISTORYWRITER_H
#define TESTSALEHISTORYWRITER_H

#include <QObject>
#include <QTest>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "SaleHistoryWriter.h"
#include "SaleHistoryParser.h"
#include "SqliteAllDataDataSelector.h"

class TestSaleHistoryWriter : public QObject
{
    Q_OBJECT
public:
    explicit TestSaleHistoryWriter(QObject *parent = 0);

private:
    void removeFile(const QString &fileName);
    void createTestDB(const QString &dbName);
    bool createTestDbStructure(QSqlDatabase &db);

private slots:
    void testSaleHistoryWriter();
    void testSaleHistoryWriter_data();

};

template <typename T>
bool compareLists(const QList<T> &act, const QList<T> &exp);

#endif // TESTSALEHISTORYWRITER_H
