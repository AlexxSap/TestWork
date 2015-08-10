#include "mainwindow.h"
#include <QApplication>
#include <QTest>

#include "tests/TestWriteReadFileToDB.h"
#include "tests/TestItemOperators.h"
#include "tests/TestSaleHistoryDayOperators.h"
#include "tests/TestSaleHistory.h"
#include "tests/TestSaleHistoryGenerator.h"

/*
///notes конечные цели:
/// 1. уметь генерировать историю продаж на заданый временной период для заданого числа артикулов и складов
/// 2. уметь читать "подряд" несколько историй продаж для заданного набора товаров
/// 3. уметь замерять за какое время записываются и считываются определённые объёмы историй продаж
/// Ниже будут описаны основные классы и сигнатуры их интерфейсов


// писалка товаров в базу
class SaleHistoryWriter
{
public:
    /// пишем в базу продажи days, возвращает false, если хотя бы один день не записался и отменяет всю запись в базу
    bool write(const QList<SaleHistoryDay> &days);
};

/// "потоковая" читалка товаров
class SalesHistoryStreamReader
{
public:
    /// собираемся читать items товары
    SalesHistoryStreamReader(const QList<Item> &items);

    /// перед чтением указывает откуда и докуда будем читать
    /// from и to могут принимать значения Date(), в этом случае читаем от минимальной или до максимальной даты для каждого товара в отдельности
    /// возврашает false, если не удалось инициализировать чтение или вообще что-то пошло не так
    /// если from и to не Date(), то SaleHistory::from() == from и SalesHistory::to() == to для каждого элемента, который выдаёт SalesHistoryStreamReader::current()
    bool open(const Date &from, const Date &to);

    /// проверяем, есть ли ещё что читать в "потоке", возвращает false если нету, возвращает true если есть и переходит к этому элементу
    bool next();

    /// возвращает текущий SaleHistory в "потоке чтения"
    /// если в "потоке" ничего не осталось, то возвращает какой-то невалидный SaleHistory
    /// для всех items из конструктора SalesHistoryStreamReader::current должен выдать какой-то SaleHistory, независимо от существования товара в базе
    SaleHistory current();
};

*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(argc == 2 && QString(argv[1]) == "-t")
    {
        TestWriteReadFileToDB testWriteReadFileToDB;
        QTest::qExec(&testWriteReadFileToDB);
        TestItemOperators testItemOperators;
        QTest::qExec(&testItemOperators);
        TestSaleHistoryDayOperators testSaleHistoryDayOperators;
        QTest::qExec(&testSaleHistoryDayOperators);
        TestSaleHistory testSaleHistory;
        QTest::qExec(&testSaleHistory);
        TestSaleHistoryGenerator testSaleHistoryGenerator;
        QTest::qExec(&testSaleHistoryGenerator);
        return 0;
    }
    else
    {
        MainWindow w;
        w.show();
        return a.exec();
    }
    return 0;
}
