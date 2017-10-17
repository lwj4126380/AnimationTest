#include "headclickablelistwidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QScrollBar>
#include <QDebug>
#include <QTimer>
#include "hoverablewidget.h"

HeadClickableListWidget::HeadClickableListWidget(bool bClickable, QString text, QVariant icons, QWidget *parent) : QWidget(parent)
  , preHoverableWidget(Q_NULLPTR)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);

    if (bClickable) {

    } else {
        QLabel *lb = new QLabel(text);
        lb->setObjectName("headLabel");
        layout->addWidget(lb);
    }

    contentWidget = new QListWidget();
    contentWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    contentWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    contentWidget->setObjectName("leftListWidget");
    contentWidget->setFrameStyle(QFrame::NoFrame);
    contentWidget->setFocusPolicy(Qt::NoFocus);
    layout->addWidget(contentWidget);
    setLayout(layout);
}

void HeadClickableListWidget::addWidgetItem(QString objName, QString text)
{
    HoverableWidget *widget = new HoverableWidget();
    widget->setTyleTwo(text, objName);
    QListWidgetItem *item = new QListWidgetItem();
    connect(widget, &HoverableWidget::clicked, this, [&](){
        HoverableWidget *hw = (HoverableWidget *)sender();
        if (hw == preHoverableWidget)
            return;

        if (preHoverableWidget != Q_NULLPTR)
            preHoverableWidget->setChecked(false);

        preHoverableWidget = hw;
    });

    contentWidget->addItem(item);
    contentWidget->setItemWidget(item, widget);
    contentWidget->setFixedHeight(32*contentWidget->count());
}

