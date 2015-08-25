#ifndef TANALOGSTABLE_H
#define TANALOGSTABLE_H

#include <QObject>
#include <QTest>

#include "TestUtility.h"
#include "AnalogsTable.h"

class TAnalogsTable : public QObject
{
    Q_OBJECT
public:
    explicit TAnalogsTable(QObject *parent = 0);

private slots:
    void TestAddAnalog();
    void TestAddAnalog_data();

    void TestIsInto();
    void TestIsInto_data();

    void TestAssign();
    void TestAssign_data();

    void TestAnalogsForProduct();
    void TestAnalogsForProduct_data();
};

#endif // TANALOGSTABLE_H
