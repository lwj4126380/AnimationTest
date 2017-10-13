#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sleekwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public SleekWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QApplication *app, bool isMainWindow = false);
    explicit MainWindow(QApplication *app, SleekWindow *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
