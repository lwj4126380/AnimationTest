#include "playwidget.h"
#include <QDebug>
#include <QStyleOption>
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

HoverAnimateButton::HoverAnimateButton(QWidget *parent) : QPushButton(parent)
{

}

void HoverAnimateButton::enterEvent(QEvent *event)
{
    Q_UNUSED(event)
    QPropertyAnimation *colorAnimationCenter = new QPropertyAnimation(this, "color");
    colorAnimationCenter->setDuration(200);
    colorAnimationCenter->setStartValue(startColor());
    colorAnimationCenter->setEndValue(endColor());
    colorAnimationCenter->setEasingCurve(QEasingCurve::OutCubic);
    colorAnimationCenter->start(QAbstractAnimation::DeleteWhenStopped);
}

void HoverAnimateButton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    QPropertyAnimation *colorAnimationCenter = new QPropertyAnimation(this, "color");
    colorAnimationCenter->setDuration(200);
    colorAnimationCenter->setStartValue(endColor());
    colorAnimationCenter->setEndValue(startColor());
    colorAnimationCenter->setEasingCurve(QEasingCurve::OutCubic);
    colorAnimationCenter->start(QAbstractAnimation::DeleteWhenStopped);
}

PlayWidget::PlayWidget(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *root = new QHBoxLayout(this);

    QHBoxLayout *hb = new QHBoxLayout();
    hb->setSpacing(24);
    HoverAnimateButton *previousBtn = new HoverAnimateButton;
    previousBtn->setObjectName("previousBtn");
    QVBoxLayout *v1 = new QVBoxLayout;
    v1->setMargin(0);
    v1->setSpacing(0);
    v1->addStretch();
    v1->addWidget(previousBtn);
    v1->addStretch();
    HoverAnimateButton *playPauseBtn = new HoverAnimateButton;
    playPauseBtn->setObjectName("playPauseBtn");
    QVBoxLayout *v2 = new QVBoxLayout;
    v2->setMargin(0);
    v2->setSpacing(0);
    v2->addStretch();
    v2->addWidget(playPauseBtn);
    v2->addStretch();
    HoverAnimateButton *nextBtn = new HoverAnimateButton;
    nextBtn->setObjectName("nextBtn");
    QVBoxLayout *v3 = new QVBoxLayout;
    v3->setMargin(0);
    v3->setSpacing(0);
    v3->addStretch();
    v3->addWidget(nextBtn);
    v3->addStretch();
    hb->addLayout(v1);
    hb->addLayout(v2);
    hb->addLayout(v3);

    QHBoxLayout *musicSliderLayout = new QHBoxLayout();
    QLabel *curTimeLabel = new QLabel("00:00");
    QLabel *durationLabel = new QLabel("00:00");
    CustomSlider *timeSlider = new CustomSlider(Qt::Horizontal);
    timeSlider->setStyle(new NewSliderStyle);
    timeSlider->setRange(0, 1000);
    musicSliderLayout->addWidget(curTimeLabel);
    musicSliderLayout->addWidget(timeSlider);
    musicSliderLayout->addWidget(durationLabel);

    root->addLayout(hb);
    root->addLayout(musicSliderLayout);
}

CustomSlider::CustomSlider(Qt::Orientation orientation, QWidget *parent) : QSlider(orientation, parent)
{

}

void CustomSlider::paintEvent(QPaintEvent *event)
{
    QSlider::paintEvent((event));
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QStyle *styl=style();
    QRect rectHandle=styl->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, NULL);
    QRect rectGroove=styl->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, NULL);

    int avl=styl->pixelMetric(QStyle::PM_SliderSpaceAvailable, &opt, this);
    QPainter p(this);
    int dd = rectGroove.width()/4;
    int offset=14;
    if (rectHandle.x() < dd)
        offset=14;
    else if (rectHandle.x() < 2*dd)
        offset -= 1;
    else if (rectHandle.x() < 3*dd)
        offset -= 2;
    else
        offset -= 3;
    QRect h = QRect(rectHandle.x()+offset, 9, 10, 4);
//    h.setX(rectHandle.x()+18);
    p.fillRect(h, QColor(194, 194, 196));
    qDebug() << "AAAAA  " << rectHandle << rectGroove << avl;
}

void PlayWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QStyleOption  opt;
    opt.init(this);
    QPainter  p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

