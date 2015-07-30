#ifndef FILEREADER_H
#define FILEREADER_H

#include <QObject>

#include "database.h"

//базовый класс для загрузки файла в базу
class FileReader : public QObject
{
    Q_OBJECT
public:
    enum Error {NoError,
                EmptyFile,
                FileNotOpen,
                FileNotLoaded,
                DBError};
public:
    explicit FileReader(QObject *parent = 0):QObject(parent){}
    virtual ~FileReader(){}

    virtual FileReader::Error readFromFile(const QString &fileName, DataBase &db)=0;


///notes чем обосновано создание всех этих сигналов?
signals:
    void started();
    void ended();
    void canceled();
    void progressRangeChanged(int minimum, int maximum);
    void progressTextChanged(const QString & progressText);
    void progressValueChanged(int progressValue);
};

#endif // FILEREADER_H
