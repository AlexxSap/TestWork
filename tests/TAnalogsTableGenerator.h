#ifndef TANALOGSTABLEGENERATOR_H
#define TANALOGSTABLEGENERATOR_H

#include <QObject>
#include <QTest>
#include <QDebug>

#include "AnalogsTableGenerator.h"

class TAnalogsTableGenerator : public QObject
{
    Q_OBJECT
public:
    explicit TAnalogsTableGenerator(QObject *parent = 0);

private slots:
    void TestGenerate();
    void TestGenerate_data();
};

#endif // TANALOGSTABLEGENERATOR_H
