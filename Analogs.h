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

    void addAnalog(const ID &analog);

    ID mainAnalog() const;
    QList<ID> toList() const;

    QString toString() const;

    bool contains(const ID &product) const;
    bool isValid() const;

    bool operator != (const Analogs &other) const;
    bool operator == (const Analogs &other) const;
};

Q_DECLARE_METATYPE(Analogs)

inline QDebug operator<< (QDebug debug, const Analogs &analogs)
{
   debug << analogs.toString();
   return debug;
}

inline uint qHash(const Analogs &key, uint seed)
{
    return qHash(key.mainAnalog(), seed);
}

#endif // ANALOGS_H
