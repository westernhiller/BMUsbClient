#include "mainwindow.h"

#include <QApplication>
#include "global.h"

int main(int argc, char *argv[])
{
    qRegisterMetaType<cv::Mat>("cv::Mat");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
