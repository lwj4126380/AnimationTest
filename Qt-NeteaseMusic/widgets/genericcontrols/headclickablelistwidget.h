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
    enum ClickableWidgetType {
        ClickableWithAddBtn,
        Clickable,
        NotClickable,
    };

public:
    explicit HeadClickableListWidget(ClickableWidgetType type, QString text, QVariant icons, QWidget *parent = 0);
    void addWidgetItem(QString objName, QString text);
    void clearSelection();

private:
    void expandListWidgetOrNot();

private slots:
    void addEditableWidget();

signals:
    void listItemClicked();

private:
    QListWidget *contentWidget;
    HoverableWidget *preHoverableWidget;
    bool bExpanded;
};

#endif // HEADCLICKABLELISTWIDGET_H
