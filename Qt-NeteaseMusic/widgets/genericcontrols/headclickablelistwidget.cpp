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
#include <QMimeData>
#include <QDrag>
#include <QPainter>
extern MainWindow *mm;

const int SongListNameId = Qt::UserRole + 1;

myViewStyle::myViewStyle(QStyle* style)
     :QProxyStyle(style)
{}

void myViewStyle::drawPrimitive ( PrimitiveElement element, const QStyleOption * option, QPainter * painter, const QWidget * widget) const{
    if (element == QStyle::PE_IndicatorItemViewItemDrop && !option->rect.isNull()){
        QStyleOption opt(*option);
        if (opt.rect.y() == 0)
            opt.rect.setY(opt.rect.y());
        else
            opt.rect.setY(opt.rect.y()-1);
        opt.rect.setHeight(2);
        opt.rect.setBottom(opt.rect.bottom()-1);
        painter->setPen(QColor(198, 47, 47));
        if (widget) opt.rect.setRight(widget->width());
        QProxyStyle::drawPrimitive(element, &opt, painter, widget);
        return;
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

ContextMenuListWidget::ContextMenuListWidget(QWidget *parent) :
    QListWidget(parent),
    isMouseMoved(false),
    startDragItem(Q_NULLPTR)
{
    setStyle(new myViewStyle(style()));
    setFocusPolicy(Qt::NoFocus);
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::InternalMove);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setSelectionMode(QAbstractItemView::NoSelection);
}

void ContextMenuListWidget::contextMenuEvent(QContextMenuEvent *event)
{
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
    w->move(mapTo(mm, event->pos()));
    w->show();
}

bool ContextMenuListWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched->objectName() == "ddd" && event->type() == QEvent::FocusOut) {
        watched->deleteLater();
    }
    return QListWidget::eventFilter(watched, event);
}

void ContextMenuListWidget::startDrag(Qt::DropActions supportedActions)
{
    qDebug() << "AAAAAAAA";
    setSelectionMode(QAbstractItemView::SingleSelection);
    Q_UNUSED(supportedActions);
    QListWidgetItem *item = startDragItem;
    QList<QListWidgetItem*> tl;
    tl.append(item);
    if (item) {
        QMimeData *data =mimeData(tl);
        if (!data)
            return;
        QDrag *drag = new QDrag(this);
        QString text = item->data(SongListNameId).toString();
        QFontMetrics fm(qApp->font());
        QSize ps = fm.boundingRect(text).size();
        QPixmap pixmap(ps.width()+16, ps.height()+10);
        pixmap.fill(Qt::transparent);

        QPen pen(QColor(138, 138, 139));
        QPainter p1(&pixmap);
        p1.setPen(pen);
        p1.setCompositionMode(QPainter::CompositionMode_Source);
        p1.drawText(pixmap.rect(), Qt::AlignCenter, text);
        p1.setCompositionMode(QPainter::CompositionMode_SourceOver);
        p1.fillRect(pixmap.rect(), QColor(252, 252, 253, 64));
        p1.setPen(QPen(QColor(214, 214, 215)));
        const qreal radius = 2;
        QPainterPath path;
        QRectF rect = QRect(0, 0, pixmap.width()-1, pixmap.height()-1);
        path.moveTo(rect.topRight() - QPointF(radius, 0));
        path.lineTo(rect.topLeft() + QPointF(radius, 0));
        path.quadTo(rect.topLeft(), rect.topLeft() + QPointF(0, radius));
        path.lineTo(rect.bottomLeft() + QPointF(0, -radius));
        path.quadTo(rect.bottomLeft(), rect.bottomLeft() + QPointF(radius, 0));
        path.lineTo(rect.bottomRight() - QPointF(radius, 0));
        path.quadTo(rect.bottomRight(), rect.bottomRight() + QPointF(0, -radius));
        path.lineTo(rect.topRight() + QPointF(0, radius));
        path.quadTo(rect.topRight(), rect.topRight() + QPointF(-radius, -0));
        p1.drawPath(path);
        p1.end();

        drag->setPixmap(pixmap);
        drag->setMimeData(data);
        drag->exec(supportedActions);
    }
}

void ContextMenuListWidget::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "1111111111111";
    isLeftButtonClicked = event->button() == Qt::LeftButton;
    startPos = event->pos();
    startDragItem = itemAt(startPos);
    isMouseMoved = false;
}

void ContextMenuListWidget::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "22222222222";
    if (!isMouseMoved) {
        setSelectionMode(QAbstractItemView::SingleSelection);
        itemAt(event->pos())->setSelected(true);
        setSelectionMode(QAbstractItemView::NoSelection);
    }
}

void ContextMenuListWidget::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "33333333333" << event->button();
    if (!isLeftButtonClicked)
        goto end;
    isMouseMoved = true;
    QPoint dis = event->pos()-startPos;
    if (dis.manhattanLength() > 5)
        setState(QAbstractItemView::DraggingState);
end:
    QListWidget::mouseMoveEvent(event);
}

void ContextMenuListWidget::dropEvent(QDropEvent *event)
{
    QListWidgetItem *endItem = itemAt(event->pos());
    int endIndex = row(endItem);
    int targetIndex = -1;
    if (event->pos().y() >= (row(endItem)+1) * 32-16)
        targetIndex = endIndex + 1;
    else
        targetIndex = endIndex;

    QListWidgetItem *il = new QListWidgetItem(*startDragItem);
    insertItem(targetIndex, il);
    setItemWidget(il, itemWidget(startDragItem));
//    il->setSelected(startDragItem->isSelected());
    blockSignals(true);
    setCurrentRow(targetIndex);
    blockSignals(false);
    qDebug() << "FFFFFFFF" << row(il);
    delete takeItem(row(startDragItem));
    setSelectionMode(QAbstractItemView::NoSelection);
}

void ContextMenuListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    QListWidgetItem *item = itemAt(event->pos());
    bool b = item->isSelected();
    if (item == startDragItem) {
        event->ignore();
    } else {
        item->setSelected(false);
        QListWidget::dragMoveEvent(event);
        item->setSelected(b);
    }
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
    contentWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    contentWidget->setObjectName("leftListWidget");
    contentWidget->setFixedHeight(0);
    contentWidget->setFrameStyle(QFrame::NoFrame);
    layout->addWidget(contentWidget);
    setLayout(layout);
}

void HeadClickableListWidget::addOrInsertWidgetItem(QString objName, QString text, bool bInsert, int index)
{
    HoverableWidget *widget = new HoverableWidget();
    widget->setTyleTwo(text, objName);
    QListWidgetItem *item = new QListWidgetItem();
    item->setData(SongListNameId, text);
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
    contentWidget->setFixedHeight(contentWidget->count() * 32);
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
    else
        contentWidget->setFixedHeight(contentWidget->count() * 32);
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
    contentWidget->setFixedHeight(contentWidget->count() * 32);
    lineEdit->setFocus();
}
