#include "headclickablelistwidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QScrollBar>
#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QLineEdit>
#include <QEvent>
#include <QMenu>
#include <QAction>
#include "hoverablewidget.h"
#include <QApplication>
#include "mainwindow.h"
#include <QGraphicsDropShadowEffect>
extern MainWindow *mm;


ContextMenuListWidget::ContextMenuListWidget(QWidget *parent) :
    QListWidget(parent)
{
    setDragEnabled(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

QSize ContextMenuListWidget::sizeHint() const {
    if (model()->rowCount() == 0) return QSize(width(), 0);
    return QSize(width(), model()->rowCount()*sizeHintForRow(0));
}

void ContextMenuListWidget::contextMenuEvent(QContextMenuEvent *event)
{

    qDebug() << currentRow();
    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);
    shadow_effect->setOffset(0, 0);
    shadow_effect->setColor(Qt::gray);
    shadow_effect->setBlurRadius(8);

    QLabel *w = new QLabel(mm);
    w->setStyleSheet("background-color: #FAFAFC; border: 1px solid #c4c4c6; border-radius: 1px;");
    w->setObjectName("ddd");
    w->setGraphicsEffect(shadow_effect);
    w->setText("FFFFFFFFFF");
    w->setFocus();
    w->installEventFilter(this);
    w->move(event->pos());
    w->show();
}

bool ContextMenuListWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched->objectName() == "ddd" && event->type() == QEvent::FocusOut) {
        watched->deleteLater();
    }
    return QListWidget::eventFilter(watched, event);
}



HeadClickableListWidget::HeadClickableListWidget(ClickableWidgetType type, QString text, QVariant icons, QWidget *parent) : QWidget(parent)
  , preHoverableWidget(Q_NULLPTR)
  , bExpanded(true)
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
        connect(pb, &QPushButton::clicked, this, &HeadClickableListWidget::expandListWidgetOrNot);
        pb->setCursor(Qt::PointingHandCursor);
        pb->setObjectName("headPushButton");
        hb->addWidget(pb);

        if (type == ClickableWithAddBtn) {
            QPushButton *addLb = new QPushButton();
            connect(addLb, &QPushButton::clicked, this, &HeadClickableListWidget::addEditableWidget);
            addLb->setCursor(Qt::PointingHandCursor);
            addLb->setObjectName("addSheetButton");
            addLb->setProperty("ButtonType", "sideWidgetHead");
            hb->addWidget(addLb);
        }

        QPushButton *arrLb = new QPushButton();
        connect(arrLb, &QPushButton::clicked, this, &HeadClickableListWidget::expandListWidgetOrNot);
        arrLb->setCursor(Qt::PointingHandCursor);
        arrLb->setObjectName("arrButton");
        arrLb->setProperty("expand", true);
        arrLb->setProperty("ButtonType", "sideWidgetHead");

        hb->addWidget(arrLb);
        layout->addLayout(hb);
    }

    contentWidget = new ContextMenuListWidget();
    contentWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    contentWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    contentWidget->setObjectName("leftListWidget");
    contentWidget->setFrameStyle(QFrame::NoFrame);
    layout->addWidget(contentWidget);
    setLayout(layout);
}

void HeadClickableListWidget::addOrInsertWidgetItem(QString objName, QString text, bool bInsert, int index)
{
    HoverableWidget *widget = new HoverableWidget();
    widget->setTyleTwo(text, objName);
    QListWidgetItem *item = new QListWidgetItem();
    connect(widget, &HoverableWidget::clicked, this, [&](){
        emit listItemClicked();
        HoverableWidget *hw = (HoverableWidget *)sender();
        if (hw == preHoverableWidget)
            return;

        if (preHoverableWidget != Q_NULLPTR)
            preHoverableWidget->setChecked(false);

        preHoverableWidget = hw;
    });

    if (!bInsert)
        contentWidget->addItem(item);
    else
        contentWidget->insertItem(index, item);
    contentWidget->setItemWidget(item, widget);
}

void HeadClickableListWidget::clearSelection()
{
    contentWidget->clearSelection();
    if (preHoverableWidget)
        preHoverableWidget->setChecked(false);

    preHoverableWidget = Q_NULLPTR;
}

bool HeadClickableListWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched->objectName() == "addSongListLineEdit" && event->type() == QEvent::FocusOut) {
        onFinishAddSongList();
    }

    if (watched->objectName() == "addSongListWidget") {
        if (QEvent::MouseButtonPress == event->type())
            return true;
    }

    return QWidget::eventFilter(watched, event);
}

void HeadClickableListWidget::expandListWidgetOrNot()
{
    QPushButton *lb = findChild<QPushButton *>("arrButton");
    if (!lb)
        return;

    lb->setProperty("expand", !bExpanded);
    contentWidget->setVisible(!bExpanded);
    bExpanded = !bExpanded;
    lb->style()->unpolish(lb);
    lb->style()->polish(lb);
    lb->update();
}

void HeadClickableListWidget::onFinishAddSongList()
{
    QLineEdit *le = findChild<QLineEdit *>("addSongListLineEdit");
    if (!le)
        return;

    contentWidget->itemWidget(contentWidget->item(1))->deleteLater();
    delete contentWidget->takeItem(1);

    if (le->text().trimmed() != "")
        addOrInsertWidgetItem("songListLabel", le->text(), true, 1);
}

void HeadClickableListWidget::addEditableWidget()
{
    QPushButton *pb = qobject_cast<QPushButton *> (sender());
    if (!pb)
        return;
    pb->setFocus();

    if (!bExpanded)
        expandListWidgetOrNot();

    QWidget *w = new QWidget;
    w->setObjectName("addSongListWidget");
    w->installEventFilter(this);
    QHBoxLayout *layout = new QHBoxLayout(w);
    layout->setSpacing(0);
    layout->setContentsMargins(45, 0, 0, 0);
    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->installEventFilter(this);
    connect(lineEdit, &QLineEdit::returnPressed, this, [=]()
    {
        lineEdit->clearFocus();
    });
    layout->addWidget(lineEdit);
    layout->addStretch();
    lineEdit->setPlaceholderText(tr("Song List Name"));
    lineEdit->setObjectName("addSongListLineEdit");
    lineEdit->setContextMenuPolicy(Qt::PreventContextMenu);
    QListWidgetItem *item = new QListWidgetItem();
    contentWidget->insertItem(1, item);
    contentWidget->setItemWidget(item, w);
    contentWidget->viewport()->adjustSize();
    lineEdit->setFocus();
}
