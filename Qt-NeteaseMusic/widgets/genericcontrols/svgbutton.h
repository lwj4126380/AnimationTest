#ifndef SVGBUTTON_H
#define SVGBUTTON_H

#include <QLabel>
#include <QSvgRenderer>
#include <QDomElement>
#include <QFile>
#include <QPainter>

class SvgLabel : public QLabel
{
    Q_OBJECT
public:
    explicit SvgLabel(QString iconPath, int iconWidth, int iconHeight, QWidget *parent=Q_NULLPTR,
                      QString normalColor="#e29595",
                      QString hoverColor="white");
    ~SvgLabel();

private:
    QPixmap loadFromSvgFile(QString filePath, int width=0, int height=0, QString color="black");
    void SetAttrRecur(QDomElement &elem, QString strtagname, QString strattr, QString strattrval);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *e);

private:
    QPixmap normalStatePixmap;
    QPixmap hoverStatePixmap;
};

#endif // SVGBUTTON_H
