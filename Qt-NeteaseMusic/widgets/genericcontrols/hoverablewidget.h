#ifndef HOVERABLEWIDGET_H
#define HOVERABLEWIDGET_H

#include <QWidget>

class HoverableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HoverableWidget(QWidget *parent = 0);

    void setTypeOne(QString text, QString btnObjName);
    void setTyleTwo(QString text, QString svgPath, int svgWidth, int svgHeight, int iconWidth, int iconHeight);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    void changeHoverStyle(bool bHover);

signals:
    void clicked();

public slots:

private:
    bool bMouseIn;
};

#endif // HOVERABLEWIDGET_H
