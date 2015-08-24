#ifndef ANALOGS_H
#define ANALOGS_H

#include <QSet>

#include "Item.h"

class Analogs
{
private:
    ID main_;
    QSet<ID> analogs_;

public:
    Analogs();
    Analogs(ID main);

    void addAnalogs(const QList<ID> &list);
    void addAnalog(const ID &analog);
    QList<ID> analogs() const;
};

#endif // ANALOGS_H
