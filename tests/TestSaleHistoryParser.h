#ifndef TESTSALEHISTORYPARSER_H
#define TESTSALEHISTORYPARSER_H

#include <QObject>
#include <QTest>

#include "SaleHistoryParser.h"

class TestSaleHistoryParser : public QObject
{
    Q_OBJECT
public:
    explicit TestSaleHistoryParser(QObject *parent = 0);

private slots:
    void testSaleHistoryParser();
    void testSaleHistoryParser_data();
};

#endif // TESTSALEHISTORYPARSER_H
