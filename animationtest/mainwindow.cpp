#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QPropertyAnimation>

#define center_width 540
#define center_hieght 200
#define margin_width 110
#define margin_height 188

#define ToLeft 0
#define ToRight 1

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}
