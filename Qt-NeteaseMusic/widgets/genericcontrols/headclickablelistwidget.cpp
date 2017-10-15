#include "headclickablelistwidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>

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

    QListWidget *listWidget = new QListWidget();
    layout->addWidget(listWidget);

    setLayout(layout);
}
