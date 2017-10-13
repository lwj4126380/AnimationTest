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

void MainWindow::on_pushButton_clicked()
{
    ui->widget->changeBannerIndex(0);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->widget->changeBannerIndex(1);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->widget->changeBannerIndex(2);
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->widget->changeBannerIndex(3);
}
