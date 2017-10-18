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
    wLayout->setSpacing(0);

    HeadClickableListWidget *recommend = new HeadClickableListWidget(HeadClickableListWidget::NotClickable, tr("Recommand"), QVariant());
    connect(recommend, &HeadClickableListWidget::listItemClicked, this, &SideBar::clearSelected);
    recommend->addWidgetItem("discoverLabel", tr("Discover Music"));
    recommend->addWidgetItem("fmLabel", tr("Personal FM"));
    recommend->addWidgetItem("mvLabel", tr("MV"));
    recommend->addWidgetItem("friendLabel", tr("Friend"));
    wLayout->addWidget(recommend);

    HeadClickableListWidget *myMusic = new HeadClickableListWidget(HeadClickableListWidget::NotClickable, tr("My Music"), QVariant());
    connect(myMusic, &HeadClickableListWidget::listItemClicked, this, &SideBar::clearSelected);
    myMusic->addWidgetItem("localLabel", tr("Local Music"));
    myMusic->addWidgetItem("dldLabel", tr("Download Manager"));
    myMusic->addWidgetItem("cloudLabel", tr("My Cloud"));
    myMusic->addWidgetItem("artistLabel", tr("My Artist"));
    wLayout->addWidget(myMusic);

    HeadClickableListWidget *createdPlayList = new HeadClickableListWidget(HeadClickableListWidget::ClickableWithAddBtn, tr("Created Play List"), QVariant());
    connect(createdPlayList, &HeadClickableListWidget::listItemClicked, this, &SideBar::clearSelected);
    createdPlayList->addWidgetItem("loveLabel", tr("My Love Music"));
    wLayout->addWidget(createdPlayList);

    HeadClickableListWidget *playListCollection = new HeadClickableListWidget(HeadClickableListWidget::Clickable, tr("Stored Play List"), QVariant());
    connect(playListCollection, &HeadClickableListWidget::listItemClicked, this, &SideBar::clearSelected);
    wLayout->addWidget(playListCollection);

    wLayout->addStretch();
    widget->setLayout(wLayout);
    layout->addWidget(scrollArea);
    setLayout(layout);
}

void SideBar::paintEvent(QPaintEvent *event)
{
    QStyleOption  opt;
    opt.init(this);
    QPainter  p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void SideBar::clearSelected()
{
    HeadClickableListWidget *hw = (HeadClickableListWidget *)sender();

    if (preSelectedWidget && preSelectedWidget != hw)
        preSelectedWidget->clearSelection();

    preSelectedWidget = hw;
}
