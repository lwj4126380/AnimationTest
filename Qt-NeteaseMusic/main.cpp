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
    dc->Open(new HttpProvider("http://7xiclj.com1.z0.glb.clouddn.com/591a63aaeca6ea635b5d7daa.mp3"));
    dc->start();
//    while (1) {
////        dc->GetBuffer();
//        QThread::msleep(500);
//    }

    return a.exec();
}
