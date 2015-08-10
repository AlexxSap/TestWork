#include "TestSaleHistoryParser.h"

template <typename T>
bool compareLists(const QList<T> &act, const QList<T> &exp)
{
    if(act.count() != exp.count())
    {
        return false;
    }

    for(int i = 0; i < act.count(); i++)
    {
        if(act.at(i) != exp.at(i))
        {
            return false;
        }
    }
    return true;
}

TestSaleHistoryParser::TestSaleHistoryParser(QObject *parent) : QObject(parent)
{

}

void TestSaleHistoryParser::testSaleHistoryParser()
{
    QFETCH(QStringList, raw);

    QFETCH(QStringList, expProductList);
    QFETCH(QStringList, expStorageList);
    QFETCH(QList<QDate>, expDateList);
    QFETCH(QList<double>, expSoldList);
    QFETCH(QList<double>, expRestList);

    SaleHistoryParser parser;
    QList<SaleHistoryDay> list = parser.parse(raw);

    QStringList actProductList;
    QStringList actStorageList;
    QList<QDate> actDateList;
    QList<double> actSoldList;
    QList<double> actRestList;
    foreach (SaleHistoryDay day, list)
    {
        actProductList.append(day.item().product());
        actStorageList.append(day.item().storage());
        actDateList.append(day.date());
        actSoldList.append(day.sold());
        actRestList.append(day.rest());
    }

    QVERIFY(compareLists(actProductList, expProductList));
    QVERIFY(compareLists(actStorageList, expStorageList));
    QVERIFY(compareLists(actDateList, expDateList));
    QVERIFY(compareLists(actSoldList, expSoldList));
    QVERIFY(compareLists(actRestList, expRestList));
}

void TestSaleHistoryParser::testSaleHistoryParser_data()
{
    QTest::addColumn<QStringList>("raw");

    QTest::addColumn<QStringList>("expProductList");
    QTest::addColumn<QStringList>("expStorageList");
    QTest::addColumn<QList<QDate> >("expDateList");
    QTest::addColumn<QList<double> >("expSoldList");
    QTest::addColumn<QList<double> >("expRestList");

    QTest::newRow("") << (QStringList() << "товар1;склад1;2015.08.10;50.0;20.0"
                          << "товар1;склад1;2015.08.11;20.0;10.0"
                          << "товар2;склад1;2015.08.09;220.0;11.0"
                          << "товар2;склад2;2015.08.10;2.0;1.0")
                      << (QStringList() << "товар1" << "товар1" << "товар2" << "товар2")
                      << (QStringList() << "склад1" << "склад1" << "склад1" << "склад2")
                      << (QList<QDate>() << QDate(2015, 8, 10)
                          << QDate(2015, 8, 11)
                          << QDate(2015, 8, 9)
                          << QDate(2015, 8, 10))
                      << (QList<double>() << 50.0 << 20.0 << 220.0 << 2.0)
                      << (QList<double>() << 20.0 << 10.0 << 11.0 << 1.0);
}

