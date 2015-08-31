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

    ///notes почему использование внешних операторов лучше, чем использование собственных операторов класса?
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

inline uint qHash(const Analogs &key, uint seed)
{
    ///notes чем обоснована такая сложная функция кэширования?
    return qHash(key.mainAnalog(), seed) ^ key.toList().count();
}

#endif // ANALOGS_H
