#include "TestFileReder.h"

TestFileReder::TestFileReder(QObject *parent) : QObject(parent)
{

}

void TestFileReder::createFile(const QString &name, const QStringList &data)
{
    QFile file(name);
    if(!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "Не удалось создать тестовый файл" << name;
        QVERIFY(false);
    }

    foreach (QString temp, data)
    {
        file.write(QString(temp+"\r\n").toLocal8Bit());
    }
    file.close();
}

void TestFileReder::removeFile(const QString &name)
{
    if(QFile::exists(name))
    {
        if(!QFile::remove(name))
        {
            qWarning() << "Не удалось удалить тестовый файл" << name;
            QVERIFY(false);
        }
    }
}

void TestFileReder::testLoadFile()
{
    QFETCH(QString, name);
    QFETCH(QStringList, data);
    QFETCH(int, expected);

    if(!name.isEmpty())
        createFile(name, data);

    CsvFileReader csvFR;
    DBForTestFileReader db;
    int actual=static_cast<int>(csvFR.readFromFile(name, db));
    removeFile(name);

    QCOMPARE(actual, expected);
}

void TestFileReder::testLoadFile_data()
{
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
                                         << (QStringList() << "товар1;2015.07.29;12.1;544.5"
                                             << "product2;2015.07.29;23.6;54.1"
                                             << "product2;2015.07.30;2;5.4")
                                         << static_cast<int>(FileReader::NoError);

    QTest::newRow("file with ugly date") << QString("uglyDate.csv")
                                         << (QStringList() << "product1;2015.09.41;3;54.5"
                                             << "product2;29.12.2015;23.6;5.1"
                                             << "товар4;2015.27.30;2;5.4")
                                         << static_cast<int>(FileReader::FileNotLoaded);

    QTest::newRow("file with uncorrect date") << QString("uncDate.csv")
                                              << (QStringList() << "product1;2015.09.31;12.1;544.5"
                                                  << "product_2!;2015.07.29;23.6;54.1"
                                                  << "product2;2015.02.30;2;5.4")
                                              << static_cast<int>(FileReader::FileNotLoaded);

    QTest::newRow("file with a negative number of product") << QString("negative.csv")
                                                         << (QStringList() << "product1;2015.07.20;.3;54.5"
                                                             << "товар2;2015.07.20;23.6;.5.1"
                                                             << "product4;2015.07.00;2;5.4")
                                                         << static_cast<int>(FileReader::FileNotLoaded);

    QTest::newRow("file with a different structure") << QString("different.csv")
                                                     << (QStringList() << "product1;20.07.2015;.3;54.5;45"
                                                         << "product2;something else;2015.07.20,23.6;.5.1"
                                                         << "56.5,wtf")
                                                     << static_cast<int>(FileReader::FileNotLoaded);
}

