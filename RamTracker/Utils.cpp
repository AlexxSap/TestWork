#include "Utils.h"

#include <iostream>

#ifdef RAM_TRACKER
#include "RamTracker.h"
#endif

QHash<QString, BenchmarkingData> Utils::bchData_;

//! BenchmarkingReport
BenchmarkingReport::BenchmarkingReport() :
    time(-1.0),
    minRam(-1.0),
    maxRam(-1.0),
    medianRam(-1.0)
{

}

//! BenchmarkingData
BenchmarkingData::BenchmarkingData() :
    startTime()
  #ifdef RAM_TRACKER
  ,ramTracker(NULL)
  #endif
{

}

BenchmarkingData::~BenchmarkingData()
{
#ifdef RAM_TRACKER
    if(ramTracker != NULL)
    {
        delete ramTracker;
    }
#endif
}

//! Utils
double Utils::_runBenchmarking(const QString &tag)
{
    if(bchData_.contains(tag))
    {
        Q_ASSERT_X(false,
                   "runBenchmarking",
                   "before runBenchmarking stop previous benchmaring by endBenchmarking");
        bchData_.remove(tag);
        return ;
    }

    bchData_[tag] = BenchmarkingData();
    bchData_[tag].startTime = QDateTime::currentDateTime();
#ifdef RAM_TRACKER
    bchData_[tag].ramTracker = new RAMTracker();
    bchData_[tag].ramTracker->start();
#endif

    QString messageStr = bchData_[tag].startTime.toString(Qt::ISODate) + ": START " + tag;
    double rs = 0.0;
#ifdef RAM_TRACKER
    rs = RAMTracker::checkRAMStatus();
    messageStr += QString(" (RAM %1 MB)").arg(RAMTracker::checkRAMStatus());
#endif

    std::cout << messageStr.toStdString() << std::endl;
    return rs;
}

BenchmarkingReport Utils::_endBenchmarking(const QString &tag, const double &startData)
{
    if(!bchData_.contains(tag))
    {
        Q_ASSERT_X(false,
                   "endBenchmarking",
                   "before endBenchmarking you must start benchmaring by runBenchmarking");
        return BenchmarkingReport();
    }

    BenchmarkingReport report;

    const QDateTime currentTime = QDateTime::currentDateTime();

    report.time = bchData_[tag].startTime.secsTo(currentTime);
#ifdef RAM_TRACKER
    bchData_[tag].ramTracker->stop();
    report.minRam = bchData_[tag].ramTracker->minRam();
    report.maxRam = bchData_[tag].ramTracker->maxRam();
    report.medianRam = bchData_[tag].ramTracker->medianRam();
#endif

    QString messageStr = currentTime.toString(Qt::ISODate) + ": END " + tag;
    messageStr += QString(" for %1s").arg(report.time);
#ifdef RAM_TRACKER
    messageStr += QString(" (RAM %1MB MIN-RAM %2MB (%5MB) MAX-RAM %3MB (%6MB) MEDIAN-RAM %4MB (%7MB))")
            .arg(RAMTracker::checkRAMStatus())
            .arg(report.minRam)
            .arg(report.maxRam)
            .arg(report.medianRam)
            .arg(report.minRam - startData)
            .arg(report.maxRam - startData)
            .arg(report.medianRam - startData);
#endif

    std::cout << messageStr.toStdString() << std::endl;

    bchData_.remove(tag);

    return report;
}
