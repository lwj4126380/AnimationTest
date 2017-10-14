#include "svgbutton.h"



void SvgLabel::SetAttrRecur(QDomElement &elem, QString strtagname, QString strattr, QString strattrval)
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

QPixmap SvgLabel::loadFromSvgFile(QString filePath, int width, int height, QString color)
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

void SvgLabel::enterEvent(QEvent *event)
{
    setPixmap(hoverStatePixmap);
    setCursor(Qt::PointingHandCursor);

    QLabel::enterEvent(event);
}

void SvgLabel::leaveEvent(QEvent *event)
{
    setPixmap(normalStatePixmap);
    setCursor(Qt::ArrowCursor);
    QLabel::enterEvent(event);
}

void SvgLabel::mousePressEvent(QMouseEvent *e)
{

}

SvgLabel::SvgLabel(QString iconPath, int iconWidth, int iconHeight, QWidget *parent,
                   QString normalColor,
                   QString hoverColor)
    : QLabel(parent)
{
    normalStatePixmap = loadFromSvgFile(iconPath, iconWidth, iconHeight, normalColor);
    hoverStatePixmap = loadFromSvgFile(iconPath, iconWidth, iconHeight, hoverColor);
    setScaledContents(false);
    setPixmap(normalStatePixmap);
    setAlignment(Qt::AlignCenter);
}

SvgLabel::~SvgLabel()
{

}
