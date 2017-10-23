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
    dc->Open(new HttpProvider("http://m10.music.126.net/20171023184116/0ed9e608f5e529f2f6acce673255e674/ymusic/6f94/6414/6a60/32a736bcb6180421ae2ae5baf1bd94af.mp3"));

//    while (1) {
////        dc->GetBuffer();
//        QThread::msleep(500);
//    }

    return a.exec();
}
