#include "mainwindow.h"
#include <QApplication>
#include <QTest>

#include "tests/TestWriteReadFileToDB.h"


///notes конечные цели:
/// 1. уметь генерировать историю продаж на заданый временной период для заданого числа артикулов и складов
/// 2. уметь читать "подряд" несколько историй продаж для заданного набора товаров
/// 3. уметь замерять за какое время записываются и считываются определённые объёмы историй продаж
/// Ниже будут описаны основные классы и сигнатуры их интерфейсов

/// тип для выражения "кол-во товара"
typedef double Amount;

/// дата
typedef QDate Date;

/// текстовый идентификатор
typedef QString ID;


/// идентификатор товара - пара Склад и Артикул
class Item
{
public:
    /// склад
    ID storage() const;

    /// артикул
    ID product() const;
};

/// продажи за конкретный день для конкретного товара
class SaleHistoryDay
{
public:
    ///идентификатор товара
    Item item() const;

    ///сколько продали товара
    Amount sold() const;

    ///сколько осталось на складе товара
    Amount rest() const;

    ///в какой день продавали
    Date date() const;
};

/// история продаж отдельного товара
class SaleHistory
{
public:
    /// идентификатор товара
    Item item() const;

    /// первый день, когда товар продали или завезли на склад
    Date from() const;

    /// последний день, когда товар продали или завезли на склад
    Date to() const;

    /// продажи товара за конкретную дату
    SaleHistoryDay day(const Date &date) const;

    /// все дни, которые продавался или завозился товар в порядке возрастания даты
    QList<SaleHistoryDay> days() const;

    /// корректная ли история продаж
    bool isValid() const;
};

/// простенький генератор историй продаж
class SaleHistoryGenerator
{
public:
    /// генерируем продажи и остатки для всех товаров на все дни от from для to для storageNum складов, на каждом из которых productNum артикулов
    QList<SaleHistoryDay> generateHistory(const Date &from,
                                          const Date &to,
                                          const int storageNum,
                                          const int productNum) const;
};

/// писалка товаров в базу
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


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(argc == 2 && QString(argv[1]) == "-t")
    {
        TestWriteReadFileToDB testWriteReadFileToDB;
        QTest::qExec(&testWriteReadFileToDB);
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
