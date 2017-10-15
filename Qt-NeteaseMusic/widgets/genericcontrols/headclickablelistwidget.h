#ifndef HEADCLICKABLELISTWIDGET_H
#define HEADCLICKABLELISTWIDGET_H

#include <QWidget>

class QListWidget;

class HeadClickableListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HeadClickableListWidget(bool bClickable, QString text, QVariant icons, QWidget *parent = 0);
    void addWidgetItem(QString svgPath, int width, int height, int iconWidth, int iconHeight, QString text);

signals:
    void contentItemHeightChanged(int height);
public slots:

private:
    QListWidget *contentWidget;
};

#endif // HEADCLICKABLELISTWIDGET_H
