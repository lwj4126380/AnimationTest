#ifndef BANNERWIDGET_H
#define BANNERWIDGET_H

#include <QWidget>
#include "autoloadpicturelabel.h"

class BannerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BannerWidget(QWidget *parent = 0);

private:
    void moveLeft();

protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);

signals:

public slots:

private:
    QVector<BannerLabel*> bannerLabels;
    QRect centerRect;
    QRect leftRect;
    QRect rightRect;
    QRect sideEndRect;
    int   previousCenterIndex;
};

#endif // BANNERWIDGET_H
