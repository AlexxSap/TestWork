#ifndef TESTSALEHISTORY_H
#define TESTSALEHISTORY_H

#include <QObject>
#include <QTest>

#include "SaleHistory.h"
#include "SaleHistoryDay.h"


class TestSaleHistory : public QObject
{
    Q_OBJECT

public:
    explicit TestSaleHistory(QObject *parent = 0);

private slots:
    void testSaleHistoryByDate();
    void testSaleHistoryByDate_data();

    void testSaleHistoryFromToDate();
    void testSaleHistoryFromToDate_data();

    void testSaleHistoryDays();
    void testSaleHistoryDays_data();
};

#endif // TESTSALEHISTORY_H
