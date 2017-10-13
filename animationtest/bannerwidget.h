#ifndef BANNERWIDGET_H
#define BANNERWIDGET_H

#include <QWidget>
#include "autoloadpicturelabel.h"

class BannerWidget : public QWidget
{
    Q_OBJECT
public:
    enum Direction{
        ToLeft,
        ToRight,
    };

    explicit BannerWidget(QWidget *parent = 0);

    void changeBannerIndex(int index);


private:
    void caclIndex(BannerWidget::Direction type, int leftBannerIndex, int &centerBannerIndex, int &rightBannerIndex, int &unvisibleBannerIndex);
    void moveLeft();
    void moveRight();
    void jumpMove(int index);
    void startAnotherAnimation(int index, QList<int> previous);

protected:
    void resizeEvent(QResizeEvent *event);

signals:

public slots:

private:
    QVector<BannerLabel*> bannerLabels;
    QRect centerRect;
    QRect leftRect;
    QRect rightRect;
    QRect sideEndRect;

    QRect centerEndRect;
    QRect leftEndRect;
    QRect rightEndRect;

    QRect leftStartRect;
    QRect centerStartRect;
    QRect rightStartRect;

    int   previousLeftIndex;
};

#endif // BANNERWIDGET_H
