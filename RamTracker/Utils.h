#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QHash>
#include <QDateTime>

#define BENCHMARKING
#define RAM_TRACKER

#ifdef BENCHMARKING
#define runBenchmarking Utils::_runBenchmarking
#define endBenchmarking Utils::_endBenchmarking
#endif

#ifndef BENCHMARKING
#define runBenchmarking Q_UNUSED
#define endBenchmarking Q_UNUSED
#endif

#ifdef RAM_TRACKER
class RAMTracker;
#endif

struct BenchmarkingReport
{
    BenchmarkingReport();

    double time;
    double minRam;
    double maxRam;
    double medianRam;
};

struct BenchmarkingData
{
    BenchmarkingData();
    ~BenchmarkingData();

    QDateTime startTime;
#ifdef RAM_TRACKER
    RAMTracker *ramTracker;
#endif
};

class Utils
{
public:
    static void _runBenchmarking(const QString &tag);
    static BenchmarkingReport _endBenchmarking(const QString &tag);

private:
    static QHash<QString, BenchmarkingData> bchData_;
};

#endif // UTILS_H
