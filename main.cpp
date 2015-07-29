#include "mainwindow.h"
#include <QApplication>
#include <QTest>

#include <TestFileReder.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(argc==2 && argv[2]=="-t"){
        QTest::qExec(new TestFileReder);
        return 0;
    }
    else{
        MainWindow w;
        w.show();
        return a.exec();
    }
}
