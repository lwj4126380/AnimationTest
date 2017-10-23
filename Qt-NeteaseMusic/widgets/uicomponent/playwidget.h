#ifndef PLAYWIDGET_H
#define PLAYWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QSlider>
#include <QProxyStyle>
#include <QStyleOptionSlider>

class NewSliderStyle : public QProxyStyle
{
public:
    int styleHint(QStyle::StyleHint hint, const QStyleOption* option = 0, const QWidget* widget = 0, QStyleHintReturn* returnData = 0) const
    {
        if (hint == QStyle::SH_Slider_AbsoluteSetButtons)
            return (Qt::LeftButton | Qt::MidButton | Qt::RightButton);
        return QProxyStyle::styleHint(hint, option, widget, returnData);
    }
};

class HoverAnimateButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(QColor startColor READ startColor WRITE setStartColor)
    Q_PROPERTY(QColor endColor READ endColor WRITE setEndColor)
public:
    HoverAnimateButton(QWidget *parent=Q_NULLPTR);
    void setColor (QColor color){
        setStyleSheet(QString("background-color: rgb(%1, %2, %3, %4);").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
    }
    QColor color(){
        return Qt::black;
    }

    void setStartColor(QColor color) { mStartColor = color;}
    QColor startColor() { return mStartColor; }
    void setEndColor(QColor color) { mEndColor = color;}
    QColor endColor() { return mEndColor; }

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private:
    QColor mStartColor;
    QColor mEndColor;
};

class CustomSlider : public QSlider{
    Q_OBJECT
public:
    CustomSlider(Qt::Orientation orientation, QWidget *parent = Q_NULLPTR);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QStyleOptionSlider opt;
};

class PlayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlayWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
};

#endif // PLAYWIDGET_H
