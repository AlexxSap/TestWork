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
   ///notes если мы хотим выразить свойство, что все дни продаж либо равны, либо не равны, то обычно оба этих оператора тестируем в одном тесте

    void TestSaleHistoryDayOperatorEqual();
    void TestSaleHistoryDayOperatorEqual_data();

    void TestSaleHistoryDayOperatorNotEqual();
    void TestSaleHistoryDayOperatorNotEqual_data();
};

#endif // TESTSALEHISTORYDAYOPERATORS_H
