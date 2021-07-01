#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Pranata House and Gallery");
    QCoreApplication::setOrganizationDomain("pranatahouse.com");
    QCoreApplication::setApplicationName("Video Downloader");
    
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
