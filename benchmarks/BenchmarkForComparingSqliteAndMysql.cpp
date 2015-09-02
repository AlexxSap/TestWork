#include "BenchmarkForComparingSqliteAndMysql.h"

void BenchmarkForComparingSqliteAndMysql::run(const int &days,
                                              const int &storages,
                                              const int &products)
{
    qInfo() << "----Benchmark For Comparing Sqlite And Mysql----";
    qInfo() << "----Sqlite Benchmark----";
    DataBaseInfo::setDataBaseType(DataBaseInfo::SQLITE);
    BenchmarkWriteRead::run(days, storages, products);

    qInfo() << "----MYSQL Benchmark----";
    DataBaseInfo::setDataBaseType(DataBaseInfo::MYSQL);
    BenchmarkWriteRead::run(days, storages, products);
}

