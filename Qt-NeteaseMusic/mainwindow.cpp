#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QApplication *app, bool isMainWindow) :
    SleekWindow(app, QString("TestWindow"), isMainWindow),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
