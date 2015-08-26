#ifndef TSALESHISTORYREADERWITHANALOGS_H
#define TSALESHISTORYREADERWITHANALOGS_H

#include <QObject>
#include <QTest>
#include <QDebug>

#include "SaleHistoryWriter.h"
#include "AnalogsWriter.h"
#include "TestUtility.h"
#include "SalesHistoryStreamReader.h"

class TSalesHistoryReaderWithAnalogs : public QObject
{
    Q_OBJECT
public:
    explicit TSalesHistoryReaderWithAnalogs(QObject *parent = 0);

private slots:
    void TestSalesHistoryReaderWithAnalogs();
    void TestSalesHistoryReaderWithAnalogs_data();
};

#endif // TSALESHISTORYREADERWITHANALOGS_H
