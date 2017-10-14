#include "svgbutton.h"
#include <QApplication>
#include <QDebug>

void SvgButton::SetAttrRecur(QDomElement &elem, QString strtagname, QString strattr, QString strattrval)
{
    // if it has the tagname then overwritte desired attribute
    if (elem.tagName().compare(strtagname) == 0)
    {
        elem.setAttribute(strattr, strattrval);
    }
    // loop all children
    for (int i = 0; i < elem.childNodes().count(); i++)
    {
        if (!elem.childNodes().at(i).isElement())
        {
            continue;
        }
        SetAttrRecur(elem.childNodes().at(i).toElement(), strtagname, strattr, strattrval);
    }
}

void SvgButton::enterEvent(QEvent *event)
{
    setIcon(QIcon(hoverStatePixmap));
    setCursor(Qt::PointingHandCursor);
    QPushButton::enterEvent(event);
}

void SvgButton::leaveEvent(QEvent *event)
{
    setIcon(QIcon(normalStatePixmap));
    setCursor(Qt::ArrowCursor);
    QPushButton::leaveEvent(event);
}

QPixmap SvgButton::loadFromSvgFile(QString filePath, int width, int height, QString color)
{
    QPixmap result;
    // open svg resource load contents to qbytearray
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return result;
    QByteArray baData = file.readAll();
    file.close();
    // load svg contents to xml document and edit contents
    QDomDocument doc;
    doc.setContent(baData);
    // recurivelly change color
    SetAttrRecur(doc.documentElement(), "path", "fill", color);
    // create svg renderer with edited contents
    QSvgRenderer svgRenderer(doc.toByteArray());
    // create pixmap target (could be a QImage)
    if (!width | !height)
        result = QPixmap(svgRenderer.defaultSize());
    else
        result = QPixmap(width, height);
    result.fill(Qt::transparent);
    // create painter to act over pixmap
    QPainter pixPainter(&result);
    // use renderer to render over painter which paints on pixmap
    svgRenderer.render(&pixPainter);
    return result;
}

SvgButton::SvgButton(QString iconPath, int iconWidth, int iconHeight,
                     QString normalColor,
                     QString hoverColor,
                     QWidget *parent)
    : QPushButton(parent)
{
    normalStatePixmap = loadFromSvgFile(iconPath, iconWidth, iconHeight, normalColor);
    hoverStatePixmap = loadFromSvgFile(iconPath, iconWidth, iconHeight, hoverColor);
    setIcon(QIcon(normalStatePixmap));
    setIconSize(QSize(iconWidth, iconHeight));
    bTextMode = false;
}

//painter.setCompositionMode(QPainter::CompositionMode_Source);
//painter.fillRect(resultImage.rect(), Qt::transparent);
//painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
//painter.drawPixmap(0, 0, head_mask);
//painter.setCompositionMode(QPainter::CompositionMode_SourceOut);
//painter.drawPixmap(0, 0, src.scaled(size));
//painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
//painter.end();

SvgButton::SvgButton(QString text, QColor normalColor, QColor hoverColor, QString iconPath, int width, int height, QWidget *parent) :
    QPushButton(parent)
{
    setText(text);
    QFontMetrics fm(qApp->font());
    QRect boundRect = fm.boundingRect(text);
    QPixmap pixmap(boundRect.width() + width, boundRect.height()+6 > height ? boundRect.height()+6 : height);
    QPainter p(&pixmap);
    p.setPen(normalColor);
    p.drawText(0, 30, text);
    p.end();
    pixmap.save("F:\\kkk.png");
}

SvgButton::~SvgButton()
{

}
