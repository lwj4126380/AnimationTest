#include "bannerwidget.h"
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QTimer>

#define center_width 540
#define center_height 200

#define side_width 513
#define side_height 190

#define animation_duration 200

#define total_banners 4

BannerWidget::BannerWidget(QWidget *parent) :
    QWidget(parent),
    previousCenterIndex(-1)
{
    BannerLabel *lb1 = new BannerLabel("http://p4.music.126.net/mp2Y2n4ueZzIj6JSnUOdtw==/7875801790676538.jpg", this);
    BannerLabel *lb3 = new BannerLabel("http://p4.music.126.net/tGPljf-IMOCyPvumoWLOTg==/7987951976374270.jpg", this);
    BannerLabel *lb2 = new BannerLabel("http://p3.music.126.net/e0gGadEhjur2UuUpDF9hPg==/7788940372125389.jpg", this);
    BannerLabel *lb4 = new BannerLabel("http://p3.music.126.net/s25q2x5QyqsAzilCurD-2w==/7973658325212564.jpg", this);
    lb4->hide();
    bannerLabels.push_back(lb1);
    bannerLabels.push_back(lb2);
    bannerLabels.push_back(lb3);
    bannerLabels.push_back(lb4);
}

void BannerWidget::moveLeft()
{
    QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup();

    int leftBannerIndex=previousCenterIndex,centerBannerIndex=-1,rightBannerIndex=-1,unvisibleBannerIndex=-1;

    switch (previousCenterIndex) {
    case total_banners-1:
        centerBannerIndex=0;
        rightBannerIndex=1;
        unvisibleBannerIndex=2;
        break;
    case total_banners-2:
        centerBannerIndex=leftBannerIndex+1;
        rightBannerIndex=0;
        unvisibleBannerIndex=1;
        break;
    case total_banners-3:
        centerBannerIndex=leftBannerIndex+1;
        rightBannerIndex=leftBannerIndex+2;
        unvisibleBannerIndex=0;
        break;
    default:
        centerBannerIndex=leftBannerIndex+1;
        rightBannerIndex=leftBannerIndex+2;
        unvisibleBannerIndex=leftBannerIndex+3;
        break;
    }

    QPropertyAnimation *colorAnimationCenter = new QPropertyAnimation(bannerLabels[centerBannerIndex], "color");
    colorAnimationCenter->setDuration(animation_duration);
    colorAnimationCenter->setStartValue(QColor(0, 0, 0, 0));
    colorAnimationCenter->setEndValue(QColor(0, 0, 0, 150));
    colorAnimationCenter->setEasingCurve(QEasingCurve::InOutQuad);

    QPropertyAnimation *animationCenter = new QPropertyAnimation(bannerLabels[centerBannerIndex], "geometry");
    animationCenter->setDuration(animation_duration);
    animationCenter->setStartValue(centerRect);
    animationCenter->setKeyValueAt(0.1, QRect(centerRect.x()+10, centerRect.y()+5, centerRect.width()-20, centerRect.height()-10));
    animationCenter->setEndValue(leftRect);
    animationCenter->setEasingCurve(QEasingCurve::InOutQuad);

    QPropertyAnimation *colorAnimationRight = new QPropertyAnimation(bannerLabels[rightBannerIndex], "color");
    colorAnimationRight->setDuration(animation_duration);
    colorAnimationRight->setStartValue(QColor(0, 0, 0, 150));
    colorAnimationRight->setEndValue(QColor(0, 0, 0, 0));
    colorAnimationRight->setEasingCurve(QEasingCurve::InOutQuad);

    QPropertyAnimation *animationRight = new QPropertyAnimation(bannerLabels[rightBannerIndex], "geometry");
    animationRight->setDuration(animation_duration);
    animationRight->setStartValue(rightRect);
    animationRight->setEndValue(centerRect);
    animationRight->setEasingCurve(QEasingCurve::InOutQuad);

    QPropertyAnimation *animationLeft = new QPropertyAnimation(bannerLabels[leftBannerIndex], "geometry");
    animationLeft->setDuration(animation_duration);
    animationLeft->setStartValue(leftRect);
    animationLeft->setEndValue(sideEndRect);
    animationLeft->setEasingCurve(QEasingCurve::InOutQuad);

    bannerLabels[unvisibleBannerIndex]->show();
    bannerLabels[unvisibleBannerIndex]->lower();
    QPropertyAnimation *animationLast = new QPropertyAnimation(bannerLabels[unvisibleBannerIndex], "geometry");
    animationLast->setDuration(animation_duration);
    animationLast->setStartValue(sideEndRect);
    animationLast->setEndValue(rightRect);
    animationLast->setEasingCurve(QEasingCurve::InOutQuad);

    animationGroup->addAnimation(animationCenter);
    animationGroup->addAnimation(animationRight);
    animationGroup->addAnimation(animationLeft);
    animationGroup->addAnimation(animationLast);
    animationGroup->addAnimation(colorAnimationRight);
    animationGroup->addAnimation(colorAnimationCenter);

    connect(animationGroup, &QParallelAnimationGroup::finished, [=](){
        bannerLabels[rightBannerIndex]->setColor(QColor(0, 0, 0, 0));
        if (previousCenterIndex == total_banners-1)
            previousCenterIndex = 0;
        else
            previousCenterIndex++;
    });

    animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
    QTimer::singleShot(animation_duration/2, [=](){
        bannerLabels[rightBannerIndex]->raise();
    });
}

void BannerWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    centerRect = QRect((width()-center_width)/2, 0, center_width, center_height);
    leftRect = QRect(0, 10, side_width, side_height);
    rightRect = QRect(width()-side_width, 10, side_width, side_height);
    sideEndRect = QRect((width()-center_width)/2, 10, center_width, center_height-10);

    if (previousCenterIndex == -1) {
        previousCenterIndex = 0;

        bannerLabels[previousCenterIndex]->setGeometry(leftRect);
        bannerLabels[previousCenterIndex+1]->setGeometry(centerRect);
        bannerLabels[previousCenterIndex+1]->setColor(QColor(0, 0, 0, 0));
        bannerLabels[previousCenterIndex+2]->setGeometry(rightRect);
    }
}

void BannerWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    moveLeft();
}
