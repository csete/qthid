#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Hanlincrest Ltd");
    QCoreApplication::setOrganizationDomain("hanlincrest.com");
    QCoreApplication::setApplicationName("FUNcube Dongle controller");

    MainWindow w;
    w.show();

    return a.exec();
}
