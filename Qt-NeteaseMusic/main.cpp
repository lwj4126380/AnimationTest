#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

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

    MainWindow w(&a, true);
    w.show();

    return a.exec();
}
