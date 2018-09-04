#include <QApplication>
#include "mainwindow.h"
#include <QDebug>

int main(int argc,char **argv)
{
    srand((unsigned)time(NULL));
    QApplication app(argc,argv);
    MainWindow w;
    w.show();
    return app.exec();
}
