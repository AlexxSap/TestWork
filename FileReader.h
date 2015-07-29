#ifndef FILEREADER_H
#define FILEREADER_H

#include <QObject>
#include <QAbstractItemModel>

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
    FileReader(QObject *parent = 0):QObject(parent){}
    virtual ~FileReader(){}

    virtual FileReader::Error readFromFile(const QString &)=0;

signals:
    void started();
    void ended();
    void canceled();
    void progressRangeChanged(int minimum, int maximum);
    void progressTextChanged(const QString & progressText);
    void progressValueChanged(int progressValue);
};

#endif // FILEREADER_H
