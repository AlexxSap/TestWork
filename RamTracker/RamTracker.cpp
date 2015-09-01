#include "RAMTracker.h"

#include <QTimer>

#ifdef Q_OS_WIN
#include <windows.h>
#include <Psapi.h>
#endif

//#include "ListUtilities.h"

RAMTracker::RAMTracker(QObject *parent) :
    QThread(parent),
    isStarted_(false)
{
}

RAMTracker::~RAMTracker()
{
    stop();
}

void RAMTracker::start()
{
    if(!isStarted_)
    {
        isStarted_ = true;
        ramList_.clear();
        ramList_ << checkRAMStatus();

        QThread::start();
    }
}

void RAMTracker::stop()
{
    if(isStarted_)
    {
        isStarted_ = false;
        while(isRunning()) { };
        ramList_ << checkRAMStatus();
    }
}

double RAMTracker::checkRAMStatus()
{
    PROCESS_MEMORY_COUNTERS memCounter;
    bool result = GetProcessMemoryInfo(GetCurrentProcess(),
                                       &memCounter,
                                       sizeof(memCounter));
    if(result)
    {
        return memCounter.WorkingSetSize / (1024 * 1024);
    }
    return -1;
}

double RAMTracker::minRam() const
{
    return ListUtilities::min(ramList_);
}

double RAMTracker::maxRam() const
{
    return ListUtilities::max(ramList_);
}

double RAMTracker::medianRam() const
{
    return ListUtilities::median(ramList_);
}

void RAMTracker::run()
{
    QDateTime startTime = QDateTime::currentDateTime();

    while(isStarted_)
    {
        if(startTime.secsTo(QDateTime::currentDateTime()) > 5)
        {
            startTime = QDateTime::currentDateTime();
            ramList_ << checkRAMStatus();
        }
    }
}

double ListUtilities::min(const QList<double> &list)
{
    if(list.isEmpty())
    {
        return 0.0;
    }
    double min = list.at(0);
    foreach (const double &val, list)
    {
        if(val < min)
        {
            min = val;
        }
    }
    return min;
}

double ListUtilities::max(const QList<double> &list)
{
    if(list.isEmpty())
    {
        return 0.0;
    }
    double max = list.at(0);
    foreach (const double &val, list)
    {
        if(val > max)
        {
            max = val;
        }
    }
    return max;
}

double ListUtilities::median(const QList<double> &list)
{
    if(list.isEmpty())
    {
        return 0.0;
    }
    double median = list.at(0);
    double sum = 0.0;
    foreach (const double &val, list)
    {
        sum += val;
    }
    median = sum / list.count();
    return median;
}
