#ifndef HEADCLICKABLELISTWIDGET_H
#define HEADCLICKABLELISTWIDGET_H

#include <QWidget>

class QListWidget;
class QListWidgetItem;
class HoverableWidget;

class HeadClickableListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HeadClickableListWidget(bool bClickable, QString text, QVariant icons, QWidget *parent = 0);
    void addWidgetItem(QString objName, QString text);

signals:
    void contentItemHeightChanged(int height);

private:
    QListWidget *contentWidget;
    HoverableWidget *preHoverableWidget;
};

#endif // HEADCLICKABLELISTWIDGET_H
