#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QObject>

#include "DataBase.h"

//базовый класс для выгрузки в файл из базы
class FileWriter : public QObject
{
    Q_OBJECT
public:
    explicit FileWriter(QObject *parent = 0);
    virtual ~FileWriter();

    virtual bool writeToFile(const QString &fileName, DataBase &ds) = 0;
};

#endif // FILEWRITER_H
