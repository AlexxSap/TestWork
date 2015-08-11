#ifndef TESTSALESHISTORYSTREAMREADER_H
#define TESTSALESHISTORYSTREAMREADER_H

#include <QObject>
#include <QTest>

#include "SalesHistoryStreamReader.h"
#include "SaleHistoryWriter.h"
#include "SaleHistoryParser.h"

class TestSalesHistoryStreamReader : public QObject
{
    Q_OBJECT
public:
    explicit TestSalesHistoryStreamReader(QObject *parent = 0);
    void removeFile(const QString &fileName);
    void createTestDB(const QString &dbName);
    bool createTestDbStructure(QSqlDatabase &db);

private slots:
    void testSalesHistoryStreamReader();
    void testSalesHistoryStreamReader_data();
};

template <typename T>
bool compareLists(const QList<T> &act, const QList<T> &exp);

#endif // TESTSALESHISTORYSTREAMREADER_H
