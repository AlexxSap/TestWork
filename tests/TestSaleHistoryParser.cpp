#include "TestSaleHistoryParser.h"

TestSaleHistoryParser::TestSaleHistoryParser(QObject *parent) : QObject(parent)
{

}

void TestSaleHistoryParser::testSaleHistoryParser()
{
    QFETCH(QStringList, raw);
    QFETCH(bool, expValid);
    QFETCH(QSet<SaleHistoryDay>, expData);

    SaleHistoryParser parser;
    const QSet<SaleHistoryDay> actData = parser.parse(raw).toSet();

    const bool actValid = parser.isValid();

    QCOMPARE(actValid, expValid);
    QCOMPARE(actData, expData);

}

void TestSaleHistoryParser::testSaleHistoryParser_data()
{
    QTest::addColumn<QStringList>("raw");
    QTest::addColumn<bool>("expValid");
    QTest::addColumn <QSet<SaleHistoryDay> >("expData");

    QTest::newRow("empty") << QStringList()
                           << false
                           << QSet<SaleHistoryDay>();

    QTest::newRow("good data") << (QStringList() << "склад1;товар1;2015.08.10;50.0;20.0"
                                   << "склад1;товар1;2015.08.11;20.0;10.0"
                                   << "склад1;товар2;2015.08.09;220.0;11.0"
                                   << "склад2;товар2;2015.08.10;2.0;1.0")
                               << true
                               << (QSet<SaleHistoryDay>()
                                   << SaleHistoryDay(Item(ID("склад1"), ID("товар1")), Date(2015, 8, 10), 50.0, 20.0)
                                   << SaleHistoryDay(Item(ID("склад1"), ID("товар1")), Date(2015, 8, 11), 20.0, 10.0)
                                   << SaleHistoryDay(Item(ID("склад1"), ID("товар2")), Date(2015, 8, 9), 220.0, 11.0)
                                   << SaleHistoryDay(Item(ID("склад2"), ID("товар2")), Date(2015, 8, 10), 2.0, 1.0));

    QTest::newRow("bad data") << (QStringList() << "склад1;товар1;2015.08.10;50.0;20.0"
                                  << "склад1;товар1;2015.08.11;20.0;10.0"
                                  << "склад1;товар2;2015.08.09;220.0;11.0"
                                  << "склад2;товар2;2015.08.10;1.0")
                              << false
                              << QSet<SaleHistoryDay>();
}
