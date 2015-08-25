#ifndef TANALOGSREADER_H
#define TANALOGSREADER_H

#include <QObject>
#include <QTest>

#include "AnalogsWriter.h"
#include "AnalogsReader.h"
#include "TestUtility.h"

class TAnalogsReader : public QObject
{
    Q_OBJECT
public:
    explicit TAnalogsReader(QObject *parent = 0);

private slots:
    void TestReadAnalogs();
    void TestReadAnalogs_data();
};

#endif // TANALOGSREADER_H
