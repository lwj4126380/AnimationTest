#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont f("Microsoft Yahei");
    a.setFont(f);

    QFile stylesheet( ":/stylesheets/red.css" );
    if ( stylesheet.open( QFile::ReadOnly ) ) {
        QString styleSheet = stylesheet.readAll();
        a.setStyleSheet( styleSheet );
    }

    MainWindow w(&a, true);
    w.show();

    return a.exec();
}
