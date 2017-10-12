#include "bannerwidget.h"
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QTimer>

#define center_width 540
#define center_height 200

#define side_width 513
#define side_height 190

#define animation_duration 200

BannerWidget::BannerWidget(QWidget *parent) : QWidget(parent)
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

    QPropertyAnimation *animationCenter = new QPropertyAnimation(bannerLabels[1], "geometry");
    connect(animationCenter, &QPropertyAnimation::finished, [&](){
        bannerLabels[1]->showMask(true);
    });
    animationCenter->setDuration(animation_duration);
    animationCenter->setStartValue(centerRect);
    animationCenter->setKeyValueAt(0.1, QRect(centerRect.x()+10, centerRect.y()+5, centerRect.width()-20, centerRect.height()-10));
    animationCenter->setEndValue(leftRect);
    animationCenter->setEasingCurve(QEasingCurve::InOutQuad);

    QPropertyAnimation *animationRight = new QPropertyAnimation(bannerLabels[2], "geometry");
    connect(animationRight, &QPropertyAnimation::finished, [&](){
        bannerLabels[2]->showMask(false);
    });
    animationRight->setDuration(animation_duration);
    animationRight->setStartValue(rightRect);
    animationRight->setEndValue(centerRect);
    animationRight->setEasingCurve(QEasingCurve::InOutQuad);

    QPropertyAnimation *animationLeft = new QPropertyAnimation(bannerLabels[0], "geometry");
    connect(animationLeft, &QPropertyAnimation::finished, [&](){
        bannerLabels[0]->showMask(true);
    });
    animationLeft->setDuration(animation_duration);
    animationLeft->setStartValue(leftRect);
    animationLeft->setEndValue(sideEndRect);
    animationLeft->setEasingCurve(QEasingCurve::InOutQuad);

    bannerLabels[3]->show();
    bannerLabels[3]->lower();
    bannerLabels[3]->lower();
    bannerLabels[3]->lower();
    QPropertyAnimation *animationLast = new QPropertyAnimation(bannerLabels[3], "geometry");
    connect(animationLast, &QPropertyAnimation::finished, [&](){
        bannerLabels[3]->showMask(true);
    });
    animationLast->setDuration(animation_duration);
    animationLast->setStartValue(sideEndRect);
    animationLast->setEndValue(rightRect);
    animationLast->setEasingCurve(QEasingCurve::InOutQuad);

    animationGroup->addAnimation(animationCenter);
    animationGroup->addAnimation(animationRight);
    animationGroup->addAnimation(animationLeft);
    animationGroup->addAnimation(animationLast);

    animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
    QTimer::singleShot(animation_duration/2, [&](){
        bannerLabels[2]->raise();
    });


}

void BannerWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    centerRect = QRect((width()-center_width)/2, 0, center_width, center_height);
    leftRect = QRect(0, 10, side_width, side_height);
    rightRect = QRect(width()-side_width, 10, side_width, side_height);
    sideEndRect = QRect((width()-center_width)/2, 10, center_width, center_height-10);

    bannerLabels[0]->setGeometry(leftRect);
    bannerLabels[1]->setGeometry(centerRect);
    bannerLabels[2]->setGeometry(rightRect);
}

void BannerWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    moveLeft();
}
