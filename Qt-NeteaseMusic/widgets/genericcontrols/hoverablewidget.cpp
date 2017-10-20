#include "hoverablewidget.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QVariant>
#include <QApplication>
#include <QStyle>

HoverableWidget::HoverableWidget(QWidget *parent) : QWidget(parent),
    bCheckedable(false),
    bChecked(false)
{

}

void HoverableWidget::setTypeOne(QString text, QString btnObjName)
{
    setProperty("HoverableWidgetType", "type1");
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setSpacing(10);
    layout->setContentsMargins(0, 0, 15, 0);
    QLabel *lb = new QLabel();
    lb->setText(text);
    layout->addWidget(lb);

    QLabel *sb = new QLabel();
    sb->setObjectName(btnObjName);
    layout->addWidget(sb);
    setLayout(layout);
}

void HoverableWidget::setTyleTwo(QString text, QString lbObjName)
{
    bCheckedable = true;
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setProperty("HoverableWidgetType", "type2");
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setSpacing(10);
    layout->setContentsMargins(18, 0, 0, 0);
    QLabel *lb = new QLabel();
    lb->setText(text);

    QLabel *lbIcon = new QLabel();
    lbIcon->setProperty("LabelType", "sideListWidgetItemIcon");
    lbIcon->setObjectName(lbObjName);
    layout->addWidget(lbIcon);
    layout->addWidget(lb);
    layout->addStretch();
    setLayout(layout);
}

void HoverableWidget::setChecked(bool b)
{
    if (!bCheckedable)
        return;

    if (b ^ bChecked) {
        changeHoverStyle(b);
        bChecked = b;
    }

    if (bChecked)
        setFocus();
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
    if (!bChecked)
        changeHoverStyle(false);

    QWidget::leaveEvent(event);
}

//void HoverableWidget::mousePressEvent(QMouseEvent *event)
//{
//    setChecked(true);

//    setFocus();
//    emit clicked();
//    if (bCheckedable)
//        QWidget::mousePressEvent(event);
//}

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
