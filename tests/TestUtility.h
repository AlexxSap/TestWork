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



    template <typename T>
    static bool compareListWithoutOrder(const QList<T> &act, const QList<T> &exp)
    {
        if(act.count() != exp.count())
        {
            return false;
        }
        foreach (const T &str, exp)
        {
            if(!act.contains(str))
            {
                return false;
            }
        }
        return true;
    }
};

#endif // TESTUTILITY_H
