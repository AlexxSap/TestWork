#ifndef TANALOGSFROMFILE_H
#define TANALOGSFROMFILE_H

#include <QObject>
#include <QTest>
#include <QDebug>

#include "AnalogsWriter.h"
#include "TestUtility.h"
#include "AnalogsReader.h"

class TAnalogsFromFile : public QObject
{
    Q_OBJECT
public:
    explicit TAnalogsFromFile(QObject *parent = 0);

private slots:
    void TestImportFromFile();
    void TestImportFromFile_data();
};

#endif // TANALOGSFROMFILE_H
