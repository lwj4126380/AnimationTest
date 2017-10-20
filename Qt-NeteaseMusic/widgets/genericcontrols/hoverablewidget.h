#ifndef HOVERABLEWIDGET_H
#define HOVERABLEWIDGET_H

#include <QWidget>
#include <QListWidgetItem>

class HoverableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HoverableWidget(QWidget *parent = 0);

    void setTypeOne(QString text, QString btnObjName);
    void setTyleTwo(QString text, QString lbObjName);

    void setChecked(bool b);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
//    void mousePressEvent(QMouseEvent *event);

private:
    void changeHoverStyle(bool bHover);

signals:
    void clicked();

public slots:

private:
    bool bCheckedable;
    bool bChecked;
    QListWidget item;
};

#endif // HOVERABLEWIDGET_H
