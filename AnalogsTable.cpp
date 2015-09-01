#include "AnalogsTable.h"

AnalogsTable::AnalogsTable()
    :table_()
{

}

AnalogsTable::AnalogsTable(const AnalogsTable &other)
    :table_()
{
    const QList<Analogs> list = other.toList();
    foreach(const Analogs &a, list)
    {
        table_.insert(a);
    }
}

void AnalogsTable::addAnalogs(const Analogs &added)
{
    table_.insert(added);
}

AnalogsTable &AnalogsTable::operator <<(const Analogs &added)
{
    table_.insert(added);
    return *this;
}

QList<Analogs> AnalogsTable::toList() const
{
    return table_.toList();
}

bool AnalogsTable::contains(const Analogs &analogs) const
{
    return table_.contains(analogs);
}

Analogs AnalogsTable::analogsForProduct(const ID &product) const
{
    if(table_.size() == 0)
    {
        return Analogs();
    }
    QList<Analogs> list = table_.toList();
    foreach(const Analogs &a, list)
    {
        if(a.contains(product))
        {
            return a;
        }
    }
    return Analogs();
}

const ID AnalogsTable::isAnalogical(const ID &left, const ID &right) const
{
    const Analogs aLeft = analogsForProduct(left);
    const Analogs aRight = analogsForProduct(right);
    if(aLeft == aRight)
    {
        return aLeft.mainAnalog();
    }
    else
    {
        return ID();
    }
}

QString AnalogsTable::toString() const
{
    QString string;
    const QList<Analogs> list = toList();
    foreach(const Analogs &a, list)
    {
        string += a.toString() + " ; ";
    }

    return string.left(string.length() - 3);
}

bool AnalogsTable::isValid() const
{
    return !table_.isEmpty();
}

bool AnalogsTable::operator != (const AnalogsTable &other) const
{
    return (table_ != other.table_);
}


bool AnalogsTable::operator == (const AnalogsTable &other) const
{
    return (table_ == other.table_);
}
