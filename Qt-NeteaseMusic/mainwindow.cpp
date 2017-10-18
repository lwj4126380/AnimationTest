#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgets/uicomponent/sidebar.h"
#include <QDebug>
#include <QSplitter>

MainWindow::MainWindow(QApplication *app, bool isMainWindow) :
    SleekWindow(app, QString("TestWindow"), isMainWindow),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QVBoxLayout *la = new QVBoxLayout;
    la->setMargin(0);
    la->setSpacing(0);
    setLayout(la);

    QSplitter *sp = new QSplitter();
    sp->setChildrenCollapsible(false);
    SideBar *sb = new SideBar;
    sp->addWidget(sb);
    QWidget *widget = new QWidget();
    widget->setStyleSheet("background-color: white");
    sp->addWidget(widget);
    la->addWidget(sp);
    sp->setStretchFactor(0, 1);
    sp->setStretchFactor(1, 10);
SleekWindow::move(200, 200);
//    centerPrimaryScreen();
}

MainWindow::MainWindow(QApplication *app, SleekWindow *parent) :
    SleekWindow(app, QString("TestWindow"), parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
