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
