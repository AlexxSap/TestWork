#ifndef SALEHISTORYWRITER_H
#define SALEHISTORYWRITER_H

#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>
#include <QSqlError>
#include <QElapsedTimer>

#include "SaleHistoryDay.h"
#include "DataBase.h"
#include "SaleHistoryParser.h"

class SaleHistoryWriter
{
private:
    DataBase db_;
    int bufferSize_;
    QSqlQuery queryForWrite_;

private:
    bool writeBuffer(const QStringList &list);

public:
    explicit SaleHistoryWriter(const QString &dbName);
    bool write(const QList<SaleHistoryDay> &days);
    bool importFromFile(const QString &fileName);

    void setBufferSize(const int size);
};

#endif // SALEHISTORYWRITER_H
