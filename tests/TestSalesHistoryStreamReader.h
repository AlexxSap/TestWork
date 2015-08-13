#ifndef TESTSALESHISTORYSTREAMREADER_H
#define TESTSALESHISTORYSTREAMREADER_H

#include <QObject>
#include <QTest>

#include "SalesHistoryStreamReader.h"
#include "SaleHistoryWriter.h"
#include "TestUtility.h"

class TestSalesHistoryStreamReader : public QObject
{
    Q_OBJECT
public:
    explicit TestSalesHistoryStreamReader(QObject *parent = 0);

private slots:
    void testSalesHistoryStreamReader();
    void testSalesHistoryStreamReader_data();
};

#endif // TESTSALESHISTORYSTREAMREADER_H
