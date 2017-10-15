#ifndef HOVERABLEWIDGET_H
#define HOVERABLEWIDGET_H

#include <QWidget>

class HoverableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HoverableWidget(QWidget *parent = 0);

    void setTypeOne(QString text, QString svgPath, int svgWidth, int svgHeight, int iconWidth, int iconHeight);
    void setTyleTwo(QString text, QString svgPath, int svgWidth, int svgHeight, int iconWidth, int iconHeight);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
    void mousePressEvent(QMouseEvent *event);

signals:
    void clicked();

public slots:

private:
    bool bMouseIn;
};

#endif // HOVERABLEWIDGET_H
