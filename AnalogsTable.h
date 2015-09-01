#ifndef ANALOGSTABLE_H
#define ANALOGSTABLE_H

#include <QSet>

#include "Analogs.h"

class AnalogsTable
{
private:
    QSet<Analogs> table_;

public:
    AnalogsTable();
    AnalogsTable(const AnalogsTable &other);

    void addAnalogs(const Analogs &added);
    AnalogsTable& operator << (const Analogs &added);
    AnalogsTable& operator = (const Analogs &added);

    QList<Analogs> toList() const;
    bool contains(const Analogs &analogs) const;

    Analogs analogsForProduct(const ID &product) const;
    const ID isAnalogical(const ID &left, const ID &right) const;

    QString toString() const;
    bool isValid() const;

    bool operator != (const AnalogsTable &other) const;
    bool operator == (const AnalogsTable &other) const;
};

Q_DECLARE_METATYPE(AnalogsTable)

inline QDebug operator<< (QDebug debug, const AnalogsTable &table)
{
   debug << table.toString();
   return debug;
}


#endif // ANALOGSTABLE_H
