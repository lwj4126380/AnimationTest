#include "hoverablewidget.h"
#include "svgbutton.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QVariant>
#include <QApplication>

HoverableWidget::HoverableWidget(QWidget *parent) : QWidget(parent),
    bMouseIn(false)
{

}

void HoverableWidget::setTypeOne(QString text, QString svgPath, int svgWidth, int svgHeight, int iconWidth, int iconHeight)
{
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setSpacing(10);
    layout->setContentsMargins(0, 0, 5, 0);
    QLabel *lb = new QLabel();
    lb->installEventFilter(this);
    lb->setObjectName("hoverWidgetLabel");
    lb->setProperty("hoverLabelTag1", true);
    lb->setText(text);
    layout->addWidget(lb);
//    lb->setEnabled(false);

    SvgButton *sb = new SvgButton(svgPath, svgWidth, svgHeight, "#eec1c1", "white");
    sb->installEventFilter(this);
    sb->setObjectName("hoverWidgetSvgButton");
    sb->resize(iconWidth, iconHeight);
    layout->addWidget(sb);
    setLayout(layout);
}

void HoverableWidget::enterEvent(QEvent *event)
{
    setCursor(Qt::PointingHandCursor);
    bMouseIn = true;

    foreach (QObject *ob, children()) {
        QEvent *e= new QEvent(QEvent::HoverEnter);
        qApp->postEvent(ob, e);

        QEvent *e1= new QEvent(QEvent::Enter);
        qApp->postEvent(ob, e1);
    }

    QWidget::enterEvent(event);
}

void HoverableWidget::leaveEvent(QEvent *event)
{
    setCursor(Qt::ArrowCursor);
    bMouseIn = false;

    foreach (QObject *ob, children()) {
        QEvent *e= new QEvent(QEvent::HoverLeave);
        qApp->postEvent(ob, e);

        QEvent *e1= new QEvent(QEvent::Leave);
        qApp->postEvent(ob, e1);
    }

    QWidget::leaveEvent(event);
}

bool HoverableWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched->objectName() == "hoverWidgetLabel" && event->type() == QEvent::HoverLeave) {
        if (bMouseIn)
            return true;
    }

    if (watched->objectName() == "hoverWidgetSvgButton" && event->type() == QEvent::Leave) {
        if (bMouseIn)
            return true;
    }

    return QWidget::eventFilter(watched, event);
}
#include <QDebug>
void HoverableWidget::mousePressEvent(QMouseEvent *event)
{
    setFocus();
}
