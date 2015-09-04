#ifndef SALEHISTORYWRITER_H
#define SALEHISTORYWRITER_H

#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>
#include <QRegExp>
//#include <QHash>

#include "SaleHistoryDay.h"
#include "DataBase.h"
#include "SaleHistoryParser.h"

class SaleHistoryWriter
{
private:
    QPointer<DataBase> db_;
    int bufferSize_;
    QSqlQuery queryForWrite_;
    QHash<int, Item> items_;
    QHash<int, Item> newItems_;
    bool isFromFile_;

private:
    bool checkFile(const QString &fileName);
    void fillItemsHashTable();
    void prepareHashTables(const QList<SaleHistoryDay> &days);

public:
    explicit SaleHistoryWriter(const DataBaseInfo &info);
    ~SaleHistoryWriter();
    bool write(const QList<SaleHistoryDay> &days);
    bool importFromFile(const QString &fileName);

    void setBufferSize(const int &size);
};

#endif // SALEHISTORYWRITER_H
