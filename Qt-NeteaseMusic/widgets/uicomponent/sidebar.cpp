#include "sidebar.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include <QVariant>
#include <QStyleOption>
#include <QPainter>
#include "headclickablelistwidget.h"
#include <QDebug>
#include <QTimer>
SideBar::SideBar(QWidget *parent) : QWidget(parent),
    preSelectedWidget(Q_NULLPTR)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(0);

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameStyle(QFrame::NoFrame);
    QWidget *widget = new QWidget();
    widget->setObjectName("sideBarWidget");
    scrollArea->setWidget(widget);

    QVBoxLayout *wLayout = new QVBoxLayout();
    wLayout->setMargin(0);
    wLayout->setSpacing(10);

    HeadClickableListWidget *recommend = new HeadClickableListWidget(ContextMenuListWidget::NotDragable, HeadClickableListWidget::NotClickable, tr("Recommand"), QVariant());
    connect(recommend, &HeadClickableListWidget::listItemClicked, this, &SideBar::clearSelected);
    recommend->addOrInsertWidgetItem("discoverLabel", tr("Discover Music"));
    recommend->addOrInsertWidgetItem("fmLabel", tr("Personal FM"));
    recommend->addOrInsertWidgetItem("mvLabel", tr("MV"));
    recommend->addOrInsertWidgetItem("friendLabel", tr("Friend"));
    wLayout->addWidget(recommend);

    HeadClickableListWidget *myMusic = new HeadClickableListWidget(ContextMenuListWidget::NotDragable, HeadClickableListWidget::NotClickable, tr("My Music"), QVariant());
    connect(myMusic, &HeadClickableListWidget::listItemClicked, this, &SideBar::clearSelected);
    myMusic->addOrInsertWidgetItem("localLabel", tr("Local Music"));
    myMusic->addOrInsertWidgetItem("dldLabel", tr("Download Manager"));
    myMusic->addOrInsertWidgetItem("cloudLabel", tr("My Cloud"));
    myMusic->addOrInsertWidgetItem("artistLabel", tr("My Artist"));
    wLayout->addWidget(myMusic);

    HeadClickableListWidget *createdPlayList = new HeadClickableListWidget(ContextMenuListWidget::CannotDragFirstItem, HeadClickableListWidget::ClickableWithAddBtn, tr("Created Play List"), QVariant());
    connect(createdPlayList, &HeadClickableListWidget::listItemClicked, this, &SideBar::clearSelected);
    createdPlayList->addOrInsertWidgetItem("loveLabel", tr("My Love Music"));
    wLayout->addWidget(createdPlayList);

    HeadClickableListWidget *playListCollection = new HeadClickableListWidget(ContextMenuListWidget::NoLimitation, HeadClickableListWidget::Clickable, tr("Stored Play List"), QVariant());
    connect(playListCollection, &HeadClickableListWidget::listItemClicked, this, &SideBar::clearSelected);
    wLayout->addWidget(playListCollection);

    wLayout->addStretch();
    widget->setLayout(wLayout);
    layout->addWidget(scrollArea); // add 播放的那个widget
    setLayout(layout);
}

void SideBar::clearSelected()
{
    HeadClickableListWidget *hw = (HeadClickableListWidget *)sender();

    if (preSelectedWidget && preSelectedWidget != hw)
        preSelectedWidget->clearSelection();

    preSelectedWidget = hw;
}
