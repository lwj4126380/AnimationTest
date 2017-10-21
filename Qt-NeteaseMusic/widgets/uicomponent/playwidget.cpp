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
    QSlider *timeSlider = new QSlider(Qt::Horizontal);
    timeSlider->setStyle(new NewSliderStyle);
    musicSliderLayout->addWidget(curTimeLabel);
    musicSliderLayout->addWidget(timeSlider);
    musicSliderLayout->addWidget(durationLabel);

    root->addLayout(hb);
    root->addLayout(musicSliderLayout);
}

void PlayWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QStyleOption  opt;
    opt.init(this);
    QPainter  p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
