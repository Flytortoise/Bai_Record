#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // qDebug() << sizeof(qint64);
    MainWindow w;
    w.show();

    return a.exec();
}
