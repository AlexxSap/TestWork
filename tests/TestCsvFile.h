#ifndef TESTCSVFILE_H
#define TESTCSVFILE_H

#include <QObject>
#include <QTest>

#include "CsvFile.h"
#include "TestUtility.h"

class TestCsvFile : public QObject
{
    Q_OBJECT
public:
    explicit TestCsvFile(QObject *parent = 0);

private slots:
    void testCsvFile();
    void testCsvFile_data();
};

#endif // TESTCSVFILE_H
