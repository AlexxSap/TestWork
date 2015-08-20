#ifndef RAMTRACKER_H

#define RAMTRACKER_H

#include <QThread>
#include <QDateTime>

class QTimer;

class ListUtilities
{
public:
    static double min(const QList<double> &list);
    static double max(const QList<double> &list);
    static double median(const QList<double> &list);
};

class RAMTracker : public QThread
{
    Q_OBJECT
public:
    RAMTracker(QObject *parent = 0);
    ~RAMTracker();

    void start();
    void stop();

    static double checkRAMStatus();

    double minRam() const;
    double maxRam() const;
    double medianRam() const;

protected:
    virtual void run();

private:
    bool isStarted_;
    QList<double> ramList_;
};

#endif // RAMTRACKER_H
