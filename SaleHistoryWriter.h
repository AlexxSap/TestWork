#ifndef SALEHISTORYWRITER_H
#define SALEHISTORYWRITER_H

#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>
#include <QSqlError>
#include <QHash>

#include "SaleHistoryDay.h"
#include "DataBase.h"
#include "SaleHistoryParser.h"

class SaleHistoryWriter
{
private:
    DataBase db_;
    int bufferSize_;
    QHash<int, Item> itemTable_;
    QHash<int, Item> deltaItemTable_;
    bool isFromFile_;
    int maxId_;

private:
    bool writeBuffer(const QStringList &list);
    bool fillItemHashTable();
    int getItemId(const Item &item);
    bool appendItemTable();

public:
    explicit SaleHistoryWriter(const QString &dbName);
    bool write(const QList<SaleHistoryDay> &days);
    bool importFromFile(const QString &fileName);

    void setBufferSize(const int size);
};

#endif // SALEHISTORYWRITER_H
