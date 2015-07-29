#include "TestFileReder.h"

TestFileReder::TestFileReder(QObject *parent) : QObject(parent){}

void TestFileReder::createFile(const QString &name, const QStringList &data){
    QFile file(name);
    if(file.open(QIODevice::WriteOnly)){
        foreach (QString temp, data) {
            file.write(QString(temp+"\r\n").toLocal8Bit());
        }
        file.close();
    }
}

void TestFileReder::removeFile(const QString &name){
    if(QFile::exists(name))
        QFile::remove(name);
}

void TestFileReder::loadFile(){
    QFETCH(QString, name);
    QFETCH(QStringList, data);
    QFETCH(int, expected);

    createFile(name, data);
    CsvFileReader csvFR;
    int actual=static_cast<int>(csvFR.readFromFile(name));
    removeFile(name);
    QCOMPARE(actual, expected);
}

void TestFileReder::loadFile_data(){
    QTest::addColumn<QString>("name");
    QTest::addColumn<QStringList>("data");
    QTest::addColumn<int>("expected");

    QTest::newRow("empty file") << QString("empFile.csv")
                                << QStringList()
                                << static_cast<int>(FileReader::EmptyFile);

    QTest::newRow("file not exist") << QString()
                                    << QStringList()
                                    << static_cast<int>(FileReader::FileNotOpen);

    QTest::newRow("correct simple file") << QString("simple.csv")
                                 << (QStringList() << "good1;2015-07-29;12.1;544.5\r\n"
                                     << "good2;2015-07-29;23.6;54.1\r\n"
                                     << "good2;2015-07-30;2;5.4")
                                 << static_cast<int>(FileReader::NoError);

    QTest::newRow("file with ugly date") << QString("uglyDate.csv")
                                 << (QStringList() << "good1;2015-07-71;3;54.5\r\n"
                                     << "good2;29.12.2015;23.6;5.1\r\n"
                                     << "good4;2015-27-30;2;5.4")
                                 << static_cast<int>(FileReader::DBError);

    QTest::newRow("file with a negative number of good") << QString("negative.csv")
                                 << (QStringList() << "good1;2015-07-20;-3;54.5\r\n"
                                     << "good2;2015-07-20;23.6;-5.1\r\n"
                                     << "good4;2015-07-00;2;5.4")
                                 << static_cast<int>(FileReader::DBError);

    QTest::newRow("file with a different structure") << QString("different.csv")
                                 << (QStringList() << "good1;20-07-2015;-3;54.5;45\r\n"
                                     << "good2;something else;2015-07-20,23.6;-5.1\r\n"
                                     << "56.5,wtf")
                                 << static_cast<int>(FileReader::FileNotLoaded);

}

