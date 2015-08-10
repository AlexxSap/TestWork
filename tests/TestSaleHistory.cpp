#include "TestSaleHistory.h"

TestSaleHistory::TestSaleHistory(QObject *parent) : QObject(parent)
{

}

void TestSaleHistory::testSaleHistoryByDate()
{
    QFETCH(QString, storage);
    QFETCH(QString, product);
    QFETCH(QList<QDate>, dateList);
    QFETCH(QList<double>, soldList);
    QFETCH(QList<double>, restList);

    QFETCH(QDate, caseDate);

    QFETCH(QString, expStorage);
    QFETCH(QString, expProduct);
    QFETCH(QDate, expDate);
    QFETCH(double, expSold);
    QFETCH(double, expRest);

    SaleHistory history(Item(storage, product));
    for(int i = 0; i < dateList.count(); i++)
    {
        history.addDay(dateList.at(i), soldList.at(i), restList.at(i));
    }

    SaleHistoryDay day = history.day(caseDate);

    QString actStorage = day.item().storage();
    QString actProduct = day.item().product();
    QDate actDate = day.date();
    double actSold = day.sold();
    double actRest = day.rest();

    QVERIFY(actStorage == expStorage);
    QVERIFY(actProduct == expProduct);
    QVERIFY(actDate == expDate);
    QVERIFY(actSold == expSold);
    QVERIFY(actRest == expRest);
}

void TestSaleHistory::testSaleHistoryByDate_data()
{
    QTest::addColumn<QString>("storage");
    QTest::addColumn<QString>("product");
    QTest::addColumn<QList<QDate> >("dateList");
    QTest::addColumn<QList<double> >("soldList");
    QTest::addColumn<QList<double> >("restList");

    QTest::addColumn<QDate>("caseDate");

    QTest::addColumn<QString>("expStorage");
    QTest::addColumn<QString>("expProduct");
    QTest::addColumn<QDate>("expDate");
    QTest::addColumn<double>("expSold");
    QTest::addColumn<double>("expRest");

    QTest::newRow("good date") << QString("storage1")
                               << QString("product1")
                               << (QList<QDate>() << QDate(2015, 8, 8)
                                   << QDate(2015, 8, 9)
                                   << QDate(2015, 8, 11))
                               << (QList<double>() << 10.0
                                   << 12.3
                                   << 10.3)
                               << (QList<double>() << 50.0
                                   << 37.7
                                   << 27.4)

                               << QDate(2015, 8, 9)

                               << QString("storage1")
                               << QString("product1")
                               << QDate(2015, 8, 9)
                               << 12.3
                               << 37.7;

    QTest::newRow("bad date") << QString("storage1")
                              << QString("product1")
                              << (QList<QDate>() << QDate(2015, 8, 8)
                                  << QDate(2015, 8, 9)
                                  << QDate(2015, 8, 11))
                              << (QList<double>() << 10.0
                                  << 12.3
                                  << 10.3)
                              << (QList<double>() << 50.0
                                  << 37.7
                                  << 27.4)

                              << QDate(2015, 8, 10)

                              << QString("storage1")
                              << QString("product1")
                              << QDate(2015, 8, 9)
                              << 12.3
                              << 37.7;

    QTest::newRow("wery bad date") << QString("storage1")
                                   << QString("product1")
                                   << (QList<QDate>() << QDate(2015, 8, 8)
                                       << QDate(2015, 8, 9)
                                       << QDate(2015, 8, 11))
                                   << (QList<double>() << 10.0
                                       << 12.3
                                       << 10.3)
                                   << (QList<double>() << 50.0
                                       << 37.7
                                       << 27.4)

                                   << QDate(2015, 8, 1)

                                   << QString("storage1")
                                   << QString("product1")
                                   << QDate(2015, 8, 1)
                                   << 0.0
                                   << 0.0;
}

void TestSaleHistory::testSaleHistoryFromToDate()
{
    QFETCH(QString, storage);
    QFETCH(QString, product);
    QFETCH(QList<QDate>, dateList);
    QFETCH(QList<double>, soldList);
    QFETCH(QList<double>, restList);

    QFETCH(QDate, expFromDate);
    QFETCH(QDate, expToDate);

    SaleHistory history(Item(storage, product));
    for(int i = 0; i < dateList.count(); i++)
    {
        history.addDay(dateList.at(i), soldList.at(i), restList.at(i));
    }

    QDate actFromDate = history.from();
    QDate actToDate = history.to();

    QVERIFY(actFromDate == expFromDate);
    QVERIFY(actToDate == expToDate);
}

void TestSaleHistory::testSaleHistoryFromToDate_data()
{
    QTest::addColumn<QString>("storage");
    QTest::addColumn<QString>("product");
    QTest::addColumn<QList<QDate> >("dateList");
    QTest::addColumn<QList<double> >("soldList");
    QTest::addColumn<QList<double> >("restList");

    QTest::addColumn<QDate>("expFromDate");
    QTest::addColumn<QDate>("expToDate");

    QTest::newRow("") << QString("storage1")
                      << QString("product1")
                      << (QList<QDate>()  << QDate(2015, 8, 14)
                          << QDate(2015, 8, 8)
                          << QDate(2015, 8, 9)
                          << QDate(2015, 8, 12)
                          << QDate(2015, 8, 11))
                      << (QList<double>() << 10.0
                          << 12.3
                          << 10.3
                          << 11.3
                          << 1.3)
                      << (QList<double>() << 50.0
                          << 37.7
                          << 27.4
                          << 27.7
                          << 17.4)
                      << QDate(2015, 8, 8)
                      << QDate(2015, 8, 14);
}


void TestSaleHistory::testSaleHistoryDays()
{
    QFETCH(QString, storage);
    QFETCH(QString, product);
    QFETCH(QList<QDate>, dateList);
    QFETCH(QList<double>, soldList);
    QFETCH(QList<double>, restList);

    QFETCH(QStringList, expStorageList);
    QFETCH(QStringList, expProductList);
    QFETCH(QList<QDate>, expDateList);
    QFETCH(QList<double>, expSoldList);
    QFETCH(QList<double>, expRestList);

    SaleHistory history(Item(storage, product));
    for(int i = 0; i < dateList.count(); i++)
    {
        history.addDay(dateList.at(i), soldList.at(i), restList.at(i));
    }

    QStringList actStorageList;
    QStringList actProductList;
    QList<QDate> actDateList;
    QList<double> actSoldList;
    QList<double> actRestList;

    QList<SaleHistoryDay> list = history.days();

    foreach (SaleHistoryDay day, list)
    {
        actStorageList.append(day.item().storage());
        actProductList.append(day.item().product());
        actDateList.append(day.date());
        actSoldList.append(day.sold());
        actRestList.append(day.rest());
    }

    QVERIFY(compareLists(actStorageList, expStorageList));
    QVERIFY(compareLists(actProductList, expProductList));
    QVERIFY(compareLists(actDateList, expDateList));
    QVERIFY(compareLists(actSoldList, expSoldList));
    QVERIFY(compareLists(actRestList, expRestList));


}

void TestSaleHistory::testSaleHistoryDays_data()
{
    QTest::addColumn<QString>("storage");
    QTest::addColumn<QString>("product");
    QTest::addColumn<QList<QDate> >("dateList");
    QTest::addColumn<QList<double> >("soldList");
    QTest::addColumn<QList<double> >("restList");

    QTest::addColumn<QStringList>("expStorageList");
    QTest::addColumn<QStringList>("expProductList");
    QTest::addColumn<QList<QDate> >("expDateList");
    QTest::addColumn<QList<double> >("expSoldList");
    QTest::addColumn<QList<double> >("expRestList");

    QTest::newRow("") << QString("storage1")
                      << QString("product1")
                      << (QList<QDate>()  << QDate(2015, 8, 14)
                          << QDate(2015, 8, 8)
                          << QDate(2015, 8, 11)
                          << QDate(2015, 8, 9))
                      << (QList<double>() << 10.0
                          << 12.3
                          << 10.3
                          << 1.3)
                      << (QList<double>() << 50.0
                          << 37.7
                          << 27.4
                          << 17.4)

                      << (QStringList() << QString("storage1")
                          << QString("storage1")
                          << QString("storage1")
                          << QString("storage1"))
                      << (QStringList() << QString("product1")
                          << QString("product1")
                          << QString("product1")
                          << QString("product1"))
                      << (QList<QDate>()  << QDate(2015, 8, 8)
                          << QDate(2015, 8, 9)
                          << QDate(2015, 8, 11)
                          << QDate(2015, 8, 14))
                      << (QList<double>() << 12.3
                          << 1.3
                          << 10.3
                          << 10.0)
                      << (QList<double>() << 37.7
                          << 17.4
                          << 27.4
                          << 50.0);
}

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
