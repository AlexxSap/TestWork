#ifndef TESTITEMOPERATORS_H
#define TESTITEMOPERATORS_H

#include <QObject>
#include <QTest>

#include "Item.h"

class TestItemOperators : public QObject
{
    Q_OBJECT
public:
    explicit TestItemOperators(QObject *parent = 0);

signals:

private slots:
    void testItemOperatorsAssign();
    void testItemOperatorsAssign_data();

    void testItemOperatorsIsEqual();
    void testItemOperatorsIsEqual_data();
};

#endif // TESTITEMOPERATORS_H
