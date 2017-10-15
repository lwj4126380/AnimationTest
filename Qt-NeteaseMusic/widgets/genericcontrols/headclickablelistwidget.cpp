#include "headclickablelistwidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include "hoverablewidget.h"

HeadClickableListWidget::HeadClickableListWidget(bool bClickable, QString text, QVariant icons, QWidget *parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
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
    contentWidget->setObjectName("leftListWidget");
    contentWidget->setFrameStyle(QFrame::NoFrame);
    contentWidget->setFocusPolicy(Qt::NoFocus);
    layout->addWidget(contentWidget);

    setLayout(layout);
}

void HeadClickableListWidget::addWidgetItem(QString svgPath, int width, int height, int iconWidth, int iconHeight, QString text)
{
    HoverableWidget *widget = new HoverableWidget();
    widget->setTyleTwo(text, svgPath, width, height, iconWidth, iconHeight);
    QListWidgetItem *item = new QListWidgetItem();
    connect(widget, &HoverableWidget::clicked, [=](){
        item->setSelected(true);
    });
    contentWidget->addItem(item);
    contentWidget->setItemWidget(item, widget);
}

