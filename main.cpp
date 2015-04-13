#include "mainwindow.h"
#include <QApplication>

#include "GlobalWeatherSoap.nsmap"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
