#include "headclickablelistwidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QScrollBar>
#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include "hoverablewidget.h"

HeadClickableListWidget::HeadClickableListWidget(ClickableWidgetType type, QString text, QVariant icons, QWidget *parent) : QWidget(parent)
  , preHoverableWidget(Q_NULLPTR)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);

    if (type == NotClickable) {
        QLabel *lb = new QLabel(text);
        lb->setObjectName("headLabel");
        layout->addWidget(lb);
    } else {
        QHBoxLayout *hb = new QHBoxLayout();
        hb->setContentsMargins(0, 0, 8, 0);
        hb->setSpacing(8);
        QPushButton *pb = new QPushButton(text);
        pb->setCursor(Qt::PointingHandCursor);
        pb->setObjectName("headPushButton");
        hb->addWidget(pb);

        if (type == ClickableWithAddBtn) {
            QPushButton *addLb = new QPushButton();
            addLb->setCursor(Qt::PointingHandCursor);
            addLb->setObjectName("addSheetButton");
            addLb->setProperty("ButtonType", "sideWidgetHead");
            hb->addWidget(addLb);
        }

        QPushButton *arrLb = new QPushButton();
        arrLb->setCursor(Qt::PointingHandCursor);
        arrLb->setObjectName("arrButton");
        arrLb->setProperty("ButtonType", "sideWidgetHead");

        hb->addWidget(arrLb);
        layout->addLayout(hb);
    }

    contentWidget = new QListWidget();
    contentWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    contentWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    contentWidget->setObjectName("leftListWidget");
    contentWidget->setFrameStyle(QFrame::NoFrame);
    contentWidget->setFocusPolicy(Qt::NoFocus);
    contentWidget->setFixedHeight(0);
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

