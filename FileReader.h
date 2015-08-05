#ifndef FILEREADER_H
#define FILEREADER_H

#include <QObject>

#include "DataBase.h"

//базовый класс для загрузки файла в базу
class FileReader : public QObject
{
    Q_OBJECT
public:
    enum Error
    {
        NoError,
        EmptyFile,
        FileNotOpen,
        FileNotLoaded,
        DBError
    };

public:
    explicit FileReader(QObject *parent = 0);
    virtual ~FileReader();

    virtual FileReader::Error readFromFile(const QString &fileName, DataBase &db) = 0;
};

#endif // FILEREADER_H
