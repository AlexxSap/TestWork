#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QObject>

#include "DataSelector.h"

//базовый класс для выгрузки в файл из базы
class FileWriter : public QObject
{
    Q_OBJECT
public:
    explicit FileWriter(QObject *parent = 0);
    virtual ~FileWriter();

    virtual bool writeToFile(const QString &fileName, DataSelector &ds)=0;

//signals:
//    void started();
//    void ended();
//    void canceled();
//    void progressRangeChanged(int minimum, int maximum);
//    void progressTextChanged(const QString & progressText);
//    void progressValueChanged(int progressValue);
};

#endif // FILEWRITER_H
