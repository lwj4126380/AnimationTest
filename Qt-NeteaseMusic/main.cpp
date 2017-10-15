#include "mainwindow.h"
#include <QApplication>

#include "headclickablelistwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QFont f("Microsoft Yahei", 9);
//    a.setFont(f);

    QFile stylesheet( ":/stylesheets/red.css" );
    if ( stylesheet.open( QFile::ReadOnly ) ) {
        QString styleSheet = stylesheet.readAll();
        a.setStyleSheet( styleSheet );
    }

    MainWindow w(&a, true);
    w.show();

//    HeadClickableListWidget hl(false, "ffff", QVariant());
//    hl.addWidgetItem(":/uiresource/svg/sidebar/discover.svg", 18, 18, 18, 18, "HHHHHHHHHHH");
//    hl.addWidgetItem(":/uiresource/svg/sidebar/discover.svg", 18, 18, 18, 18, "HHHHHHHHHHH");
//    hl.show();

    return a.exec();
}
