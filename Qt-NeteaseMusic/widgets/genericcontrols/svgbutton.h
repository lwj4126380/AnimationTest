#ifndef SVGBUTTON_H
#define SVGBUTTON_H

#include <QPushButton>
#include <QSvgRenderer>
#include <QDomElement>
#include <QFile>
#include <QPainter>

class SvgButton : public QPushButton
{
    Q_OBJECT

public:
    explicit SvgButton(QString iconPath, int iconWidth, int iconHeight,
                      QString normalColor,
                      QString hoverColor,
                      QWidget *parent=Q_NULLPTR);

    explicit SvgButton(QString text, QColor normalColor, QColor hoverColor, QString iconPath, int width, int height, QWidget *parent=Q_NULLPTR);
    ~SvgButton();

private:
    QPixmap loadFromSvgFile(QString filePath, int width=0, int height=0, QString color="black");
    void SetAttrRecur(QDomElement &elem, QString strtagname, QString strattr, QString strattrval);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private:
    QPixmap normalStatePixmap;
    QPixmap hoverStatePixmap;
    bool bTextMode;
};

#endif // SVGBUTTON_H
