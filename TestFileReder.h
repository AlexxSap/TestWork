#ifndef TESTFILEREDER_H
#define TESTFILEREDER_H

#include <QObject>
#include <QTest>
#include <QStringList>

#include "FileReader.h"
#include "CsvFileReader.h"

class TestFileReder : public QObject
{
    Q_OBJECT
public:
    explicit TestFileReder(QObject *parent = 0);
private:
    void createFile(const QString &name, const QStringList &data);
    void removeFile(const QString &name);
private slots:
    void loadFile();
    void loadFile_data();
};

#endif // TESTFILEREDER_H
