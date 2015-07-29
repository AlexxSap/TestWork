#ifndef TESTFILEREDER_H
#define TESTFILEREDER_H

#include <QObject>
#include <QTest>
#include <QStringList>

class TestFileReder : public QObject
{
    Q_OBJECT
public:
    explicit TestFileReder(QObject *parent = 0);
private:
    void createFile(QString name, QStringList data);
    void removeFile(QString name);
private slots:
    void loadFile();
    void loadFile_data();
};

#endif // TESTFILEREDER_H
