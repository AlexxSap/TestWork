#ifndef ANALOGSWRITER_H
#define ANALOGSWRITER_H

#include <QDebug>
#include <QFile>
#include <QTextCodec>

#include "AnalogsTable.h"
#include "database.h"

class AnalogsWriter
{
private:
    QPointer<DataBase> db_;

public:
    explicit AnalogsWriter(const QString &dbName);
    ~AnalogsWriter();
    bool write(const AnalogsTable &table);
    bool importFromFile(const QString &fileName);
};

#endif // ANALOGSWRITER_H
