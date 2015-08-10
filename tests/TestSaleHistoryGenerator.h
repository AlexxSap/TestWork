#ifndef TESTSALEHISTORYGENERATOR_H
#define TESTSALEHISTORYGENERATOR_H

#include <QObject>
#include <QTest>

#include "SaleHistoryGenerator.h"

class TestSaleHistoryGenerator : public QObject
{
    Q_OBJECT
private:
    bool compareData(const QList<SaleHistoryDay> &list, int maxVal);

public:
    explicit TestSaleHistoryGenerator(QObject *parent = 0);

private slots:
    void testSaleHistoryGenerator();
    void testSaleHistoryGenerator_data();

};

#endif // TESTSALEHISTORYGENERATOR_H
