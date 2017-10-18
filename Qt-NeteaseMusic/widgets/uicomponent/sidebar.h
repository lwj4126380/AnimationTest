#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>

class HeadClickableListWidget;

class SideBar : public QWidget
{
    Q_OBJECT
public:
    explicit SideBar(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
    void clearSelected();


private:
    HeadClickableListWidget *preSelectedWidget;
};

#endif // SIDEBAR_H
