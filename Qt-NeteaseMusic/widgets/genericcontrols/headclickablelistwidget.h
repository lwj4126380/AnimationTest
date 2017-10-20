#ifndef HEADCLICKABLELISTWIDGET_H
#define HEADCLICKABLELISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QProxyStyle>

class QListWidget;
class QListWidgetItem;
class HoverableWidget;

class myViewStyle: public QProxyStyle{
public:
    myViewStyle(QStyle* style = 0);

    void drawPrimitive ( PrimitiveElement element, const QStyleOption * option, QPainter * painter, const QWidget * widget = 0 ) const;
};


class ContextMenuListWidget : public QListWidget {
    Q_OBJECT

public:
    enum DragType {
        NotDragable,
        CannotDragFirstItem,
        NoLimitation,
    };

public:
    explicit ContextMenuListWidget(DragType type, QWidget *parent = Q_NULLPTR);
    void clearCheckedStatus();

protected:
    void contextMenuEvent(QContextMenuEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
    void startDrag(Qt::DropActions supportedActions);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dropEvent(QDropEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
//    void dragEnterEvent(QDragEnterEvent *event);

private:
    void setItemWidgetStatus(HoverableWidget *widget);

private:
    bool isMouseMoved;
    QPoint startPos;
    bool isLeftButtonClicked;
    QListWidgetItem *startDragItem;
    HoverableWidget *preHoverableWidget;
    DragType  dragType;

signals:
    void userSelectItem();
};

class HeadClickableListWidget : public QWidget
{
    Q_OBJECT

public:
    enum ClickableWidgetType {
        ClickableWithAddBtn,
        Clickable,
        NotClickable,
    };

public:
    explicit HeadClickableListWidget(ContextMenuListWidget::DragType dtype, ClickableWidgetType type, QString text, QVariant icons, QWidget *parent = 0);
    void addOrInsertWidgetItem(QString objName, QString text, bool bInsert=false, int index=-1);
    void clearSelection();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    void expandListWidgetOrNot();
    void onFinishAddSongList();

private slots:
    void addEditableWidget();

signals:
    void listItemClicked();

private:
    ContextMenuListWidget *contentWidget;
    bool bExpanded;
};

#endif // HEADCLICKABLELISTWIDGET_H
