#include "mainwindow.h"
#include <QApplication>
#include <QTest>

#include "tests/TestFileReder.h"
#include "tests/TestWriteReadFileToDB.h"

#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(argc==2 && QString(argv[1])== "-t"){
       // QTest::qExec(new TestFileReder);
        QTest::qExec(new TestWriteReadFileToDB);
        return 0;
    }
    else{
        MainWindow w;
        w.show();
        return a.exec();
    }

     return 0;
}
