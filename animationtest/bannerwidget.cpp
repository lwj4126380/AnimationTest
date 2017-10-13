#include "bannerwidget.h"
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QTimer>

#define center_width 540
#define center_height 200

#define side_width 513
#define side_height 190

#define center_scaledown_width 470
#define center_scaledown_height 170

#define animation_duration 25000

#define total_banners 4

BannerWidget::BannerWidget(QWidget *parent) :
    QWidget(parent),
    previousLeftIndex(-1)
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

void BannerWidget::caclIndex(BannerWidget::Direction type, int leftBannerIndex, int &centerBannerIndex, int &rightBannerIndex, int &unvisibleBannerIndex)
{
    if (type == BannerWidget::ToLeft) {
        switch (leftBannerIndex) {
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
    } else if (type == BannerWidget::ToRight) {
        switch (leftBannerIndex) {
        case total_banners-1:
            centerBannerIndex=0;
            rightBannerIndex=1;
            unvisibleBannerIndex=leftBannerIndex-1;
            break;
        case total_banners-2:
            centerBannerIndex=leftBannerIndex+1;
            rightBannerIndex=0;
            unvisibleBannerIndex=leftBannerIndex-1;
            break;
        case 0:
            centerBannerIndex=leftBannerIndex+1;
            rightBannerIndex=leftBannerIndex+2;
            unvisibleBannerIndex=total_banners-1;
            break;
        default:
            centerBannerIndex=leftBannerIndex+1;
            rightBannerIndex=leftBannerIndex+2;
            unvisibleBannerIndex=leftBannerIndex-1;
            break;
        }
    }
}

void BannerWidget::moveLeft()
{
    QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup();

    int leftBannerIndex=previousLeftIndex,centerBannerIndex=-1,rightBannerIndex=-1,unvisibleBannerIndex=-1;
    caclIndex(BannerWidget::ToLeft, leftBannerIndex, centerBannerIndex, rightBannerIndex, unvisibleBannerIndex);

    QPropertyAnimation *colorAnimationCenter = new QPropertyAnimation(bannerLabels[centerBannerIndex], "color");
    colorAnimationCenter->setDuration(animation_duration);
    colorAnimationCenter->setStartValue(QColor(0, 0, 0, 0));
    colorAnimationCenter->setEndValue(QColor(0, 0, 0, 150));
    colorAnimationCenter->setEasingCurve(QEasingCurve::InOutQuad);

    QPropertyAnimation *animationCenter = new QPropertyAnimation(bannerLabels[centerBannerIndex], "geometry");
    animationCenter->setDuration(animation_duration);
    animationCenter->setStartValue(centerRect);
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
        bannerLabels[leftBannerIndex]->hide();
        if (previousLeftIndex == total_banners-1)
            previousLeftIndex = 0;
        else
            previousLeftIndex++;
    });

    animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
    QTimer::singleShot(animation_duration/2, [=](){
        bannerLabels[rightBannerIndex]->raise();
    });
}

void BannerWidget::moveRight()
{
    QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup();

    int leftBannerIndex=previousLeftIndex,centerBannerIndex=-1,rightBannerIndex=-1,unvisibleBannerIndex=-1;
    caclIndex(BannerWidget::ToRight, leftBannerIndex, centerBannerIndex, rightBannerIndex, unvisibleBannerIndex);

    QPropertyAnimation *colorAnimationCenter = new QPropertyAnimation(bannerLabels[centerBannerIndex], "color");
    colorAnimationCenter->setDuration(animation_duration);
    colorAnimationCenter->setStartValue(QColor(0, 0, 0, 0));
    colorAnimationCenter->setEndValue(QColor(0, 0, 0, 150));
    colorAnimationCenter->setEasingCurve(QEasingCurve::InOutQuad);

    QPropertyAnimation *animationCenter = new QPropertyAnimation(bannerLabels[centerBannerIndex], "geometry");
    animationCenter->setDuration(animation_duration);
    animationCenter->setStartValue(centerRect);
    animationCenter->setEndValue(rightRect);
    animationCenter->setEasingCurve(QEasingCurve::InOutQuad);

    QPropertyAnimation *animationRight = new QPropertyAnimation(bannerLabels[rightBannerIndex], "geometry");
    animationRight->setDuration(animation_duration);
    animationRight->setStartValue(rightRect);
    animationRight->setEndValue(sideEndRect);
    animationRight->setEasingCurve(QEasingCurve::InOutQuad);

    QPropertyAnimation *colorAnimationLeft = new QPropertyAnimation(bannerLabels[leftBannerIndex], "color");
    colorAnimationLeft->setDuration(animation_duration);
    colorAnimationLeft->setStartValue(QColor(0, 0, 0, 150));
    colorAnimationLeft->setEndValue(QColor(0, 0, 0, 0));
    colorAnimationLeft->setEasingCurve(QEasingCurve::InOutQuad);

    QPropertyAnimation *animationLeft = new QPropertyAnimation(bannerLabels[leftBannerIndex], "geometry");
    animationLeft->setDuration(animation_duration);
    animationLeft->setStartValue(leftRect);
    animationLeft->setEndValue(centerRect);
    animationLeft->setEasingCurve(QEasingCurve::InOutQuad);

    bannerLabels[unvisibleBannerIndex]->show();
    bannerLabels[unvisibleBannerIndex]->lower();
    QPropertyAnimation *animationLast = new QPropertyAnimation(bannerLabels[unvisibleBannerIndex], "geometry");
    animationLast->setDuration(animation_duration);
    animationLast->setStartValue(sideEndRect);
    animationLast->setEndValue(leftRect);
    animationLast->setEasingCurve(QEasingCurve::InOutQuad);

    animationGroup->addAnimation(animationCenter);
    animationGroup->addAnimation(animationRight);
    animationGroup->addAnimation(animationLeft);
    animationGroup->addAnimation(animationLast);
    animationGroup->addAnimation(colorAnimationLeft);
    animationGroup->addAnimation(colorAnimationCenter);

    connect(animationGroup, &QParallelAnimationGroup::finished, [=](){
        bannerLabels[leftBannerIndex]->setColor(QColor(0, 0, 0, 0));
        bannerLabels[rightBannerIndex]->hide();
        if (previousLeftIndex == 0)
            previousLeftIndex = total_banners-1;
        else
            previousLeftIndex--;
    });

    animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
    QTimer::singleShot(animation_duration/2, [=](){
        bannerLabels[leftBannerIndex]->raise();
    });
}

void BannerWidget::jumpMove(int index)
{
    QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup();

    int leftBannerIndex=previousLeftIndex,centerBannerIndex=-1,rightBannerIndex=-1,unvisibleBannerIndex=-1;
    caclIndex(BannerWidget::ToRight, leftBannerIndex, centerBannerIndex, rightBannerIndex, unvisibleBannerIndex);
    QList<int> indexs;
    indexs << leftBannerIndex << centerBannerIndex << rightBannerIndex;

    QPropertyAnimation *colorAnimationCenter = new QPropertyAnimation(bannerLabels[centerBannerIndex], "color");
    colorAnimationCenter->setDuration(animation_duration*0.2);
    colorAnimationCenter->setStartValue(QColor(0, 0, 0, 0));
    colorAnimationCenter->setEndValue(QColor(0, 0, 0, 150));
    colorAnimationCenter->setEasingCurve(QEasingCurve::InOutQuad);

    QPropertyAnimation *animationCenter = new QPropertyAnimation(bannerLabels[centerBannerIndex], "geometry");
    animationCenter->setDuration(animation_duration*0.2);
    animationCenter->setStartValue(centerRect);
    animationCenter->setEndValue(centerEndRect);
    animationCenter->setEasingCurve(QEasingCurve::InOutQuad);

    QPropertyAnimation *animationRight = new QPropertyAnimation(bannerLabels[rightBannerIndex], "geometry");
    animationRight->setDuration(animation_duration*0.2);
    animationRight->setStartValue(rightRect);
    animationRight->setEndValue(rightEndRect);
    animationRight->setEasingCurve(QEasingCurve::InOutQuad);

    QPropertyAnimation *animationLeft = new QPropertyAnimation(bannerLabels[leftBannerIndex], "geometry");
    animationLeft->setDuration(animation_duration*0.2);
    animationLeft->setStartValue(leftRect);
    animationLeft->setEndValue(leftEndRect);
    animationLeft->setEasingCurve(QEasingCurve::InOutQuad);

    animationGroup->addAnimation(animationCenter);
    animationGroup->addAnimation(animationRight);
    animationGroup->addAnimation(animationLeft);
    animationGroup->addAnimation(colorAnimationCenter);

    connect(animationGroup, &QParallelAnimationGroup::finished, [=](){
        bannerLabels[centerBannerIndex]->lower();
        bannerLabels[leftBannerIndex]->lower();
        bannerLabels[rightBannerIndex]->lower();

        startAnotherAnimation(index, indexs);
    });

    animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
}

void BannerWidget::startAnotherAnimation(int index, QList<int> previous)
{
    QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup();

    int leftBannerIndex=index,centerBannerIndex=-1,rightBannerIndex=-1,unvisibleBannerIndex=-1;
    caclIndex(BannerWidget::ToLeft, leftBannerIndex, centerBannerIndex, rightBannerIndex, unvisibleBannerIndex);

    QList<int> currentIndexs;
    currentIndexs << leftBannerIndex << centerBannerIndex << rightBannerIndex;

    foreach (int index, currentIndexs) {
        if (!previous.contains(index))
            bannerLabels[index]->show();
    }
    bannerLabels[leftBannerIndex]->raise();
    bannerLabels[rightBannerIndex]->raise();
    bannerLabels[centerBannerIndex]->raise();

    QPropertyAnimation *colorAnimationCenter = new QPropertyAnimation(bannerLabels[centerBannerIndex], "color");
    colorAnimationCenter->setDuration(animation_duration*0.8);
    colorAnimationCenter->setStartValue(QColor(0, 0, 0, 150));
    colorAnimationCenter->setEndValue(QColor(0, 0, 0, 0));
    colorAnimationCenter->setEasingCurve(QEasingCurve::InCubic);

    QPropertyAnimation *animationCenter = new QPropertyAnimation(bannerLabels[centerBannerIndex], "geometry");
    animationCenter->setDuration(animation_duration*0.8);
    animationCenter->setStartValue(centerStartRect);
    animationCenter->setEndValue(centerRect);
    animationCenter->setEasingCurve(QEasingCurve::InCubic);

    QPropertyAnimation *animationRight = new QPropertyAnimation(bannerLabels[rightBannerIndex], "geometry");
    animationRight->setDuration(animation_duration*0.8);
    animationRight->setStartValue(rightStartRect);
    animationRight->setEndValue(rightRect);
    animationRight->setEasingCurve(QEasingCurve::InCubic);

    QPropertyAnimation *animationLeft = new QPropertyAnimation(bannerLabels[leftBannerIndex], "geometry");
    animationLeft->setDuration(animation_duration*0.8);
    animationLeft->setStartValue(leftStartRect);
    animationLeft->setEndValue(leftRect);
    animationLeft->setEasingCurve(QEasingCurve::InCubic);

    animationGroup->addAnimation(animationCenter);
    animationGroup->addAnimation(animationRight);
    animationGroup->addAnimation(animationLeft);
    animationGroup->addAnimation(colorAnimationCenter);

    connect(animationGroup, &QParallelAnimationGroup::finished, [=](){
        foreach (int index, previous) {
            if (!previous.contains(index))
                bannerLabels[index]->hide();
        }
        bannerLabels[centerBannerIndex]->setColor(QColor(0, 0, 0, 0));
        previousLeftIndex = leftBannerIndex;
    });

    animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
}

void BannerWidget::changeBannerIndex(int index)
{
    if (index == previousLeftIndex)
        return;

    if (previousLeftIndex == total_banners-1 && index==0) {
        moveLeft();
        return;
    }

    if (previousLeftIndex == 0 && index==total_banners-1) {
        moveRight();
        return;
    }

    if (qAbs(index-previousLeftIndex) > 1)
        jumpMove(index);
    else
        index > previousLeftIndex ? moveLeft() : moveRight();
}

void BannerWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    centerRect = QRect((width()-center_width)/2, 0, center_width, center_height);
    leftRect = QRect(0, 10, side_width, side_height);
    rightRect = QRect(width()-side_width, 10, side_width, side_height);
    sideEndRect = QRect((width()-center_width)/2, 10, center_width, center_height-10);

    centerEndRect = QRect((width()-center_scaledown_width)/2, (height()-center_scaledown_height)/2, center_scaledown_width, center_scaledown_height);
    leftEndRect = QRect(centerEndRect.x()-100, centerEndRect.y()+10, centerEndRect.width()-200, centerEndRect.height()-20);
    rightEndRect = sideEndRect;

    centerStartRect = QRect(centerEndRect.x()-5, centerEndRect.y()-3, centerEndRect.width()+10, centerEndRect.height()+6);
    leftStartRect = QRect(leftEndRect.x()+20, leftEndRect.y()-5, leftEndRect.width()+40, leftEndRect.height()+10);
    rightStartRect = QRect(rightEndRect.x()+5, rightEndRect.y()+10, rightEndRect.width()-10, rightEndRect.height()-20);

    if (previousLeftIndex == -1)
        previousLeftIndex = 0;

    int centerBannerIndex=-1,rightBannerIndex=-1,unvisibleBannerIndex=-1;
    caclIndex(BannerWidget::ToLeft, previousLeftIndex, centerBannerIndex, rightBannerIndex, unvisibleBannerIndex);
    bannerLabels[previousLeftIndex]->setGeometry(leftRect);
    bannerLabels[centerBannerIndex]->setGeometry(centerRect);
    bannerLabels[centerBannerIndex]->setColor(QColor(0, 0, 0, 0));
    bannerLabels[rightBannerIndex]->setGeometry(rightRect);
}
