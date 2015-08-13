#include "TestSaleHistoryParser.h"

TestSaleHistoryParser::TestSaleHistoryParser(QObject *parent) : QObject(parent)
{

}

void TestSaleHistoryParser::testSaleHistoryParser()
{
    QFETCH(QStringList, raw);
    QFETCH(bool, expValid);
    QFETCH(QList<SaleHistoryDay>, expList);

    SaleHistoryParser parser;
    const QList<SaleHistoryDay> list = parser.parse(raw);
    const bool actValid = parser.isValid();

    QCOMPARE(actValid, expValid);
    QCOMPARE(list, expList);
}

void TestSaleHistoryParser::testSaleHistoryParser_data()
{
    QTest::addColumn<QStringList>("raw");
    QTest::addColumn<bool>("expValid");
    QTest::addColumn<QList<SaleHistoryDay> >("expList");

    QTest::newRow("good data") << (QStringList() << "товар1;склад1;2015.08.10;50.0;20.0"
                                   << "товар1;склад1;2015.08.11;20.0;10.0"
                                   << "товар2;склад1;2015.08.09;220.0;11.0"
                                   << "товар2;склад2;2015.08.10;2.0;1.0")
                               << true
                               << (QList<SaleHistoryDay>()
                                   << SaleHistoryDay(Item(ID("склад1"), ID("товар1")), Date(2015, 8, 10), 50.0, 20.0)
                                   << SaleHistoryDay(Item(ID("склад1"), ID("товар1")), Date(2015, 8, 11), 20.0, 10.0)
                                   << SaleHistoryDay(Item(ID("склад1"), ID("товар2")), Date(2015, 8, 9), 220.0, 11.0)
                                   << SaleHistoryDay(Item(ID("склад2"), ID("товар2")), Date(2015, 8, 10), 2.0, 1.0));

    QTest::newRow("bad data") << (QStringList() << "товар1;склад1;2015.08.10;50.0;20.0"
                                  << "товар1;склад1;2015.08.11;20.0;10.0"
                                  << "товар2;склад1;2015.08.09;220.0;11.0"
                                  << "товар2;склад2;2015.08.10;1.0")
                              << false
                              << QList<SaleHistoryDay>();
}
