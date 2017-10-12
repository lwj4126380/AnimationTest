#include "mainwindow.h"
#include <QApplication>
#include "autoloadpicturelabel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    BannerLabel label("http://p4.music.126.net/mp2Y2n4ueZzIj6JSnUOdtw==/7875801790676538.jpg");
//    label.resize(640, 400);
//    label.show();


    MainWindow w;
    w.show();

    return a.exec();
}
