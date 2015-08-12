#include "TestSaleHistoryGenerator.h"

TestSaleHistoryGenerator::TestSaleHistoryGenerator(QObject *parent) : QObject(parent)
{

}

///notes что бы выбрать, что нужно обязательно тестировать, а что нет, можно задаться вопросом
/// "а что будет, если эта компонента будет работать неверно и как быстро это будет замечено".
/// что будет, если эта довольно сложная функция(цикл и четыре условия) будет работать неверно и как быстро это будет замечено?
bool TestSaleHistoryGenerator::compareData(const QList<SaleHistoryDay> &list,
                                           int maxVal)
{
    ///notes  в итераторах тоже используем const& где только можно ( foreach(const SaleHistoryDay &day, list) )
    foreach (SaleHistoryDay day, list)
    {
        if(day.sold() < 0
                || day.sold() > maxVal
                || day.rest() < 0
                || day.rest() > maxVal)
        {
            return false;
        }
    }
    return true;
}

void TestSaleHistoryGenerator::testSaleHistoryGenerator()
{
    QFETCH(QDate, fromDate);
    QFETCH(QDate, toDate);
    QFETCH(int, storageNum);
    QFETCH(int, productNum);
    QFETCH(int, maxVal);

    SaleHistoryGenerator gen;
    gen.setMaxValue(maxVal);
    QList<SaleHistoryDay> list = gen.generateHistory(fromDate,
                                                     toDate,
                                                     storageNum,
                                                     productNum);

    ///notes всякие арифметические операции в тестах обычно их сильно усложняют.
    /// тут будет лучше вынести actNum в тестовые данные, для явного указания.
    /// в тестовых данных его вполне можно задавать сложным термом без переменных, например, 10 * 20 * (43 + 1)
    int actNum = storageNum * productNum * (fromDate.daysTo(toDate) + 1);
    bool comp = compareData(list, maxVal);
    ///notes для таких вещей обычно пользуемся QCOMPARE
    QVERIFY(list.count() == actNum);
    QVERIFY(comp);
}

void TestSaleHistoryGenerator::testSaleHistoryGenerator_data()
{
    QTest::addColumn<QDate>("fromDate");
    QTest::addColumn<QDate>("toDate");
    QTest::addColumn<int>("storageNum");
    QTest::addColumn<int>("productNum");
    QTest::addColumn<int>("maxVal");

    QTest::newRow("gen test 1") << QDate(2015, 3, 1)
                                << QDate(2015, 3, 10)
                                << 3
                                << 5
                                << 10000;

    QTest::newRow("gen test 2") << QDate(2015, 2, 20)
                                << QDate(2015, 3, 4)
                                << 3
                                << 15
                                << 1000;
}

