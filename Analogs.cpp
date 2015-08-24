#include "Analogs.h"

bool operator != (const Analogs &left, const Analogs &right)
{
    return (left.main_ != right.main_ || left.analogs_ != right.analogs_);
}

bool operator == (const Analogs &left, const Analogs &right)
{
    return (left.main_ == right.main_ && left.analogs_ == right.analogs_);
}

Analogs::Analogs()
    : main_(),
      analogs_()
{

}

Analogs::Analogs(const ID &main)
    : main_(main),
      analogs_()
{

}

Analogs::Analogs(const Analogs &other)
    : main_(),
      analogs_()
{
    main_ = other.mainAnalog();
    QList<ID> al = other.analogs();
    foreach (const ID &i, al)
    {
        analogs_.insert(i);
    }
}

Analogs &Analogs::operator =(const Analogs &other)
{
    main_ = other.mainAnalog();
    QList<ID> al = other.analogs();
    foreach (const ID &i, al)
    {
        analogs_.insert(i);
    }
    return *this;
}

Analogs &Analogs::operator <<(const ID &analog)
{
    addAnalog(analog);
    return *this;
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

ID Analogs::mainAnalog() const
{
    return main_;
}

QList<ID> Analogs::analogs() const
{
    return analogs_.toList();
}

QString Analogs::toString() const
{
    QString string(main_ + " [");
    const QList<ID> analogs = analogs_.toList();
    foreach (const ID &pro, analogs)
    {
        string += pro + ", ";
    }
    if(!analogs_.isEmpty())
    {
        string = string.left(string.length() - 2);
    }

    return string + "]";
}

bool Analogs::isAnalog(const ID &product) const
{
    return analogs_.contains(product);
}
