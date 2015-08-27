#include "TestUtility.h"

bool TestUtility::createFile(const QString &fileName, const QStringList &data)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    foreach (const QString &temp, data)
    {
        file.write(QString(temp + "\r\n").toLocal8Bit());
    }
    file.close();
    return true;
}

bool TestUtility::removeFile(const QString &fileName)
{
    if(QFile::exists(fileName))
    {
        if(!QFile::remove(fileName))
        {
            return false;
        }
    }
    QThread::msleep(100); // просто винда не всегда успевает удалить файл
    return true;
}

QList<Item> TestUtility::genRandomItemList(const int storages, const int products)
{
    QString productPrefix_("prod_");
    QString storagePrefix_("stor_");

    const int maxStogareCount = storages>1?storages/2:storages;
    const int maxProductCount = products>1?products/2:products;

    QList<Item> list;
    for(int storIndex = 0; storIndex < maxStogareCount; storIndex++)
    {
        for(int prodIndex = 0; prodIndex < maxProductCount; prodIndex++)
        {
            const int storNum = rand() % storages;
            const int prodNum = rand() % products;
            const Item item(storagePrefix_ + QString::number(storNum),
                            productPrefix_ + QString::number(prodNum));

            if(list.contains(item))
            {
                prodIndex--;
            }
            else
            {
                list.append(item);

            }
        }
    }
    return list;
}
