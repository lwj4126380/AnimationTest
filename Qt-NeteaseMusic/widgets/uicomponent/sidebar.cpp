#include "sidebar.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include <QVariant>
#include <QStyleOption>
#include <QPainter>
#include "headclickablelistwidget.h"
#include <QDebug>
#include <QTimer>
SideBar::SideBar(QWidget *parent) : QWidget(parent)
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

    HeadClickableListWidget *recommend = new HeadClickableListWidget(false, tr("Recommand"), QVariant());
    recommend->addWidgetItem(":/uiresource/svg/sidebar/discover.svg", 18, 18, 18, 18, tr("Discover Music"));
    recommend->addWidgetItem(":/uiresource/svg/sidebar/fm.svg", 18, 18, 18, 18, tr("Personal FM"));
    recommend->addWidgetItem(":/uiresource/svg/sidebar/mv.svg", 18, 18, 18, 18, tr("MV"));
    recommend->addWidgetItem(":/uiresource/svg/sidebar/friend.svg", 18, 18, 18, 18, tr("Friend"));
    wLayout->addWidget(recommend);

    HeadClickableListWidget *myMusic = new HeadClickableListWidget(false, tr("My Music"), QVariant());
    myMusic->addWidgetItem(":/uiresource/svg/sidebar/local.svg", 18, 18, 18, 18, tr("Local Music"));
    myMusic->addWidgetItem(":/uiresource/svg/sidebar/dld.svg", 18, 18, 18, 18, tr("Down Manager"));
    myMusic->addWidgetItem(":/uiresource/svg/sidebar/cloud.svg", 18, 18, 18, 18, tr("My Cloud"));
    myMusic->addWidgetItem(":/uiresource/svg/sidebar/artist.svg", 18, 18, 18, 18, tr("My Artist"));
    wLayout->addWidget(myMusic);

    HeadClickableListWidget *createdPlayList = new HeadClickableListWidget(false, tr("Created Play List"), QVariant());
    wLayout->addWidget(createdPlayList);

    HeadClickableListWidget *playListCollection = new HeadClickableListWidget(false, tr("Stored Play List"), QVariant());
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
