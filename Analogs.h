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
    explicit Analogs(const ID &main);
    Analogs(const Analogs &other);

    Analogs& operator = (const Analogs &other);
    Analogs& operator << (const ID &analog);

    void addAnalogs(const QList<ID> &list);
    void addAnalog(const ID &analog);

    ID mainAnalog() const;
    QList<ID> analogs() const;

    QString toString() const;

    bool isAnalog(const ID &product) const;

    friend bool operator != (const Analogs &left, const Analogs &right);
    friend bool operator == (const Analogs &left, const Analogs &right);
};

Q_DECLARE_METATYPE(Analogs)

bool operator != (const Analogs &left, const Analogs &right);
bool operator == (const Analogs &left, const Analogs &right);


inline QDebug operator<< (QDebug debug, const Analogs &analogs)
{
   debug << analogs.toString();
   return debug;
}

#endif // ANALOGS_H
