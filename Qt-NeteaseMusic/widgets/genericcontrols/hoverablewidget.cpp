#include "hoverablewidget.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QVariant>
#include <QApplication>
#include <QStyle>

HoverableWidget::HoverableWidget(QWidget *parent) : QWidget(parent),
    bMouseIn(false)
{

}

void HoverableWidget::setTypeOne(QString text, QString btnObjName)
{
    setProperty("HoverableWidgetType", "type1");
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setSpacing(10);
    layout->setContentsMargins(0, 0, 15, 0);
    QLabel *lb = new QLabel();
    lb->installEventFilter(this);
    lb->setObjectName("hoverWidgetLabel");
    lb->setProperty("hoverLabelTag1", true);
    lb->setText(text);
    layout->addWidget(lb);

    QLabel *sb = new QLabel();
    sb->setObjectName(btnObjName);
    layout->addWidget(sb);
    setLayout(layout);
}

void HoverableWidget::setTyleTwo(QString text, QString svgPath, int svgWidth, int svgHeight, int iconWidth, int iconHeight)
{
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setSpacing(10);
    layout->setContentsMargins(18, 0, 0, 0);
    QLabel *lb = new QLabel();
    lb->installEventFilter(this);
    lb->setObjectName("hoverWidgetLabel");
    lb->setProperty("hoverLabelTag2", true);
    lb->setText(text);

//    SvgButton *sb = new SvgButton(svgPath, svgWidth, svgHeight, "#5c5c5c", "black");
//    sb->installEventFilter(this);
//    sb->setObjectName("hoverWidgetSvgButton");
//    sb->resize(iconWidth, iconHeight);
//    layout->addWidget(sb);
    layout->addWidget(lb);
    layout->addStretch();
    setLayout(layout);
}

void HoverableWidget::enterEvent(QEvent *event)
{
    setCursor(Qt::PointingHandCursor);
    changeHoverStyle(true);

    QWidget::enterEvent(event);
}

void HoverableWidget::leaveEvent(QEvent *event)
{
    setCursor(Qt::ArrowCursor);
    changeHoverStyle(false);

    QWidget::leaveEvent(event);
}

void HoverableWidget::mousePressEvent(QMouseEvent *event)
{
    setFocus();
    emit clicked();
}

void HoverableWidget::changeHoverStyle(bool bHover)
{
    foreach (QObject *ob, children()) {
        if (bHover)
            ob->setProperty("State", "hover");
        else
            ob->setProperty("State", "");


        QLabel *lb = qobject_cast<QLabel *>(ob);
        if (lb) {
            lb->style()->unpolish(lb);
            lb->style()->polish(lb);
            lb->update();
        }
    }
}
