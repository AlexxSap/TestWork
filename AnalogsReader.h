#ifndef ANALOGSREADER_H
#define ANALOGSREADER_H

#include "AnalogsTable.h"
#include "database.h"

#include <QSqlRecord>

class AnalogsReader
{
private:
    QPointer<DataBase> db_;

private:
    bool fillTempIdTable(const QList<ID> &IdList);
    AnalogsTable fetchTable();

public:
    explicit AnalogsReader(const DataBaseInfo &info);
    ~AnalogsReader();
    AnalogsTable fetch(const QList<ID> &IdList);
};

#endif // ANALOGSREADER_H
