#include "mainwindow.h"
#include <QApplication>

#include "widgets/genericcontrols/headclickablelistwidget.h"

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
//    HeadClickableListWidget *createdPlayList = new HeadClickableListWidget(HeadClickableListWidget::ClickableWithAddBtn, ("Created Play List"), QVariant());
////    connect(createdPlayList, &HeadClickableListWidget::listItemClicked, this, &SideBar::clearSelected);
//    createdPlayList->addOrInsertWidgetItem("loveLabel", ("My Love Music"));
//    createdPlayList->show();

//    SideBar sb;
//    sb.showMaximized();

    return a.exec();
}
