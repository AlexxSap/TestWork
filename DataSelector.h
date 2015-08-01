#ifndef DATASELECTOR_H
#define DATASELECTOR_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMetaType>

class DataSelector
{
public:
    DataSelector();
    virtual ~DataSelector();

    virtual QSqlQuery get();
};

Q_DECLARE_METATYPE(DataSelector)

#endif // DATASELECTOR_H
