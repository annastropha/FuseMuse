#include "mainwindow.h"
#include <QApplication>

MainWindow* mw;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    mw = &w; //declared in mainwindow.h
    w.show();

    return a.exec();
}
