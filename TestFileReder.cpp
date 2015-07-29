#include "TestFileReder.h"

TestFileReder::TestFileReder(QObject *parent) : QObject(parent){}

void TestFileReder::createFile(QString name, QStringList data){
    QFile file(name);
    if(file.open(QIODevice::WriteOnly)){
        foreach (QString temp, data) {
            file.write(QString(temp+"\r\n").toLocal8Bit());
        }
        file.close();
    }
}

void TestFileReder::removeFile(QString name){
    if(QFile::exists(name))
        QFile::remove(name);
}

void TestFileReder::loadFile(){
    QFETCH(QString, name);
    QFETCH(QStringList, data);
    QFETCH(int, expected);

    createFile(name, data);
    int actual=FileReader.readFromFile(name);
    QCOMPARE(actual, expected);
}

void TestFileReder::loadFile_data(){
    QTest::addColumn<QString>("name");
    QTest::addColumn<QStringList>("data");
    QTest::addColumn<int>("expected");

    QTest::newRow("empty file") << QString("empFile.csv")
                                << QStringList()
                                << FileReader::EmptyFile;

}

