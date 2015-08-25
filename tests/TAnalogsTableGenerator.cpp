#include "TAnalogsTableGenerator.h"

TAnalogsTableGenerator::TAnalogsTableGenerator(QObject *parent) : QObject(parent)
{

}

void TAnalogsTableGenerator::TestGenerate()
{
    QFETCH(int, groupNum);
    QFETCH(int, analogsNumMax);

    AnalogsTableGenerator gen;
    const AnalogsTable table = gen.generateTable(groupNum, analogsNumMax);

    const QList<Analogs> list = table.toList();

    const int groupCount = list.count();
    QCOMPARE(groupCount, groupNum);

    foreach (Analogs an, list)
    {
        if(an.toList().count() > analogsNumMax)
        {
            QFAIL("analogsNumMax not ok");
        }
    }
}

void TAnalogsTableGenerator::TestGenerate_data()
{
    QTest::addColumn<int>("groupNum");
    QTest::addColumn<int>("analogsNumMax");

    QTest::newRow("") << 50
                      << 23;
}

