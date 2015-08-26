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

    friend bool operator != (const AnalogsTable &left, const AnalogsTable &right);
    friend bool operator == (const AnalogsTable &left, const AnalogsTable &right);
};

Q_DECLARE_METATYPE(AnalogsTable)

bool operator != (const AnalogsTable &left, const AnalogsTable &right);
bool operator == (const AnalogsTable &left, const AnalogsTable &right);

inline QDebug operator<< (QDebug debug, const AnalogsTable &table)
{
   debug << table.toString();
   return debug;
}


#endif // ANALOGSTABLE_H
