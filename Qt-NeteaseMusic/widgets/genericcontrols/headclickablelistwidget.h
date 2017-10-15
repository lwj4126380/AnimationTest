#ifndef HEADCLICKABLELISTWIDGET_H
#define HEADCLICKABLELISTWIDGET_H

#include <QWidget>

class HeadClickableListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HeadClickableListWidget(bool bClickable, QString text, QVariant icons, QWidget *parent = 0);

signals:

public slots:
};

#endif // HEADCLICKABLELISTWIDGET_H
