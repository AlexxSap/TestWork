#ifndef TESTSALEHISTORYDAYOPERATORS_H
#define TESTSALEHISTORYDAYOPERATORS_H

#include <QObject>
#include <QTest>
#include <QDate>

#include "SaleHistoryDay.h"

class TestSaleHistoryDayOperators : public QObject
{
    Q_OBJECT
public:
    explicit TestSaleHistoryDayOperators(QObject *parent = 0);

private slots:
    void TestSaleHistoryDayOperatorEqual();
    void TestSaleHistoryDayOperatorEqual_data();

    void TestSaleHistoryDayOperatorNotEqual();
    void TestSaleHistoryDayOperatorNotEqual_data();
};

#endif // TESTSALEHISTORYDAYOPERATORS_H
