#ifndef TANALOGPRODUCT_H
#define TANALOGPRODUCT_H

#include <QObject>
#include <QTest>

#include "TestUtility.h"
#include "Item.h"
#include "Analogs.h"

class TAnalogs : public QObject
{
    Q_OBJECT
public:
    explicit TAnalogs(QObject *parent = 0);

private slots:
    void TestAnalogsList();
    void TestAnalogsList_data();

    void TestIsAnalog();
    void TestIsAnalog_data();

    void TestAnalogsAssign();
    void TestAnalogsAssign_data();
};

#endif // TANALOGPRODUCT_H
