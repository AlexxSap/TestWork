#ifndef ANALOGSREADER_H
#define ANALOGSREADER_H

#include "AnalogsTable.h"
#include "database.h"

#include <QSqlRecord>

class AnalogsReader
{
private:
    DataBase db_;

private:
    bool createTempIdTable();
    void dropTempIdTable();
    bool fillTempIdTable(const QList<ID> IdList);

    ///notes в названии метода лучше выбирать глаголы, которые как-то указываются на "тяжесть" операции.
    /// например, если надо что-то считывать из базы, то используем fetch. для чтения из файла read. для относительно долгих расчётов calc или generate
    AnalogsTable getTable();

public:
    explicit AnalogsReader(const QString &dbName);
    ///notes всё что можно передать по константной ссылке передаём по констатной ссылке
    AnalogsTable read(const QList<ID> IdList);
};

#endif // ANALOGSREADER_H
