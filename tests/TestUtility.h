#ifndef TESTUTILITY_H
#define TESTUTILITY_H

#include <QSqlDatabase>
#include <QFile>
#include <QSqlQuery>
#include <QThread>

#include "Item.h"

class TestUtility
{
public:
    static bool createFile(const QString &fileName, const QStringList &data);
    static bool removeFile(const QString &fileName);

    static QList<Item> genRandomItemList(const int storages,
                                         const int products);
};

#endif // TESTUTILITY_H
