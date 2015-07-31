#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QObject>

#include "database.h"

//базовый класс для выгрузки в файл из базы
class FileWriter : public QObject
{
    Q_OBJECT
public:
    explicit FileWriter(QObject *parent = 0);
    virtual ~FileWriter();

    virtual bool writeToFileAllData(const QString &fileName, DataBase &db)=0;

//signals:
//    void started();
//    void ended();
//    void canceled();
//    void progressRangeChanged(int minimum, int maximum);
//    void progressTextChanged(const QString & progressText);
//    void progressValueChanged(int progressValue);
};

#endif // FILEWRITER_H
