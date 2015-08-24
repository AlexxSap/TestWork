#include "Analogs.h"

Analogs::Analogs()
    : main_(),
      analogs_()
{

}

Analogs::Analogs(ID main)
    : main_(main),
      analogs_()
{

}

void Analogs::addAnalogs(const QList<ID> &list)
{
    foreach (const QString &analog, list)
    {
        analogs_.insert(analog);
    }
}

void Analogs::addAnalog(const ID &analog)
{
    analogs_.insert(analog);
}

QList<ID> Analogs::analogs() const
{
    QList<ID> list = analogs_.toList();
    list.insert(0, main_);
    return list;
}
