#include "mainwindow.h"
#include <QApplication>
#include <QTest>

#include "tests/TestFileReder.h"
#include "tests/TestWriteReadFileToDB.h"

#include <QSqlDatabase>

///notes общее впечатление: пишешь много лишнего. не факт, что всё это пригодиться в будущем
/// ориентируйся больше на тесты и что им надо, что бы проходиться
/// имея достаточную базу тестов, явное дублирование кода, черезмерно объёмные методы и классы можно делать рефакторинг
///
/// лучший код - никакого кода. худший код - неиспользуемый

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ///notes обычно делаем два варианта запуска тестов: выборочный(те тесты, которые нужны сейчас) и полный(все тесты, какие только можно вообще)
    ///notes перед каждым коммитом в мастер запускаем все тесты. всё должно проходиться(сейчас тут это не критично)
    if(argc==2 && QString(argv[1])== "-t"){
       // QTest::qExec(new TestFileReder);
        QTest::qExec(new TestWriteReadFileToDB); ///notes зачем тут указатель?
        return 0;
    }
    else{
        MainWindow w;
        w.show();
        return a.exec();
    }

     return 0;
}
