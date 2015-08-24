#include "AnalogsTable.h"

AnalogsTable::AnalogsTable()
    :table_()
{

}

AnalogsTable::AnalogsTable(const AnalogsTable &other)
    :table_()
{
    const QList<Analogs> list = other.toList();
    foreach(const Analogs a, list)
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

QString AnalogsTable::toString() const
{
    QString string;
    const QList<Analogs> list = toList();
    foreach(const Analogs a, list)
    {
        string += a.toString() + " ; ";
    }

    return string.left(string.length() - 3);
}

bool operator !=(const AnalogsTable &left, const AnalogsTable &right)
{
    return (left.table_ != right.table_);
}


bool operator ==(const AnalogsTable &left, const AnalogsTable &right)
{
    return (left.table_ == right.table_);
}
