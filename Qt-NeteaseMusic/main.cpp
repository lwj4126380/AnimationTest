#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include "mpg123decoder.h"
#include "httpprovider.h"
#include <QThread>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //    QFont f("Microsoft Yahei", 9);
    //    a.setFont(f);

    QTranslator translator;
    translator.load(":/zhCN.qm");
    a.installTranslator(&translator);

    QFile stylesheet( ":/stylesheets/red.css" );
    if ( stylesheet.open( QFile::ReadOnly ) ) {
        QString styleSheet = stylesheet.readAll();
        a.setStyleSheet( styleSheet );
    }

//    MainWindow w(&a, true);
//    w.show();

    Mpg123Decoder *dc = new Mpg123Decoder();
    dc->Open(new HttpProvider("http://m10.music.126.net/20171023175008/c18897adcc04b79ac7e3762be188d0c7/ymusic/82cf/9884/0c60/d5c5bdb436ef45c22fa2b4e85a9ecdbb.mp3"));

//    while (1) {
////        dc->GetBuffer();
//        QThread::msleep(500);
//    }

    return a.exec();
}
