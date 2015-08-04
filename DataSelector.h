#ifndef DATASELECTOR_H
#define DATASELECTOR_H

#include <QSqlDatabase>
#include <QSqlQuery>

class DataSelector
{
public:
    DataSelector();
    virtual ~DataSelector();

    virtual QSqlQuery get();
};

#endif // DATASELECTOR_H
