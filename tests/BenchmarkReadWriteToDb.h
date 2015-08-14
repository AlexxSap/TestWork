#ifndef BENCHMARKREADWRITETODB_H
#define BENCHMARKREADWRITETODB_H

#include <QObject>
#include <QTest>

#include "SaleHistoryDay.h"
#include "TestUtility.h"
#include "SaleHistoryGenerator.h"
#include "SaleHistoryWriter.h"
#include "SalesHistoryStreamReader.h"

class BenchmarkReadWriteToDb : public QObject
{
    Q_OBJECT
public:
    explicit BenchmarkReadWriteToDb(QObject *parent = 0);

private:
    void prepare();

private slots:
    void testWrite();
    void testWrite_data();

    void testRead();
    void testRead_data();
};

#endif // BENCHMARKREADWRITETODB_H
