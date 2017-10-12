#ifndef AUTOLOADPICTURELABEL_H
#define AUTOLOADPICTURELABEL_H

#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class AutoLoadPictureLabel : public QLabel
{
    Q_OBJECT
public:
    AutoLoadPictureLabel(QString url, QWidget *parent=Q_NULLPTR);
    ~AutoLoadPictureLabel();

private:
    QPixmap pixmap;
    QNetworkAccessManager *network;
};

class BannerLabel : public AutoLoadPictureLabel
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ color WRITE setColor)

public:
    BannerLabel(QString url, QWidget *parent=Q_NULLPTR);
    ~BannerLabel();

    void setColor (QColor color){
        maskWidget->setStyleSheet(QString("background-color: rgb(%1, %2, %3, %4);").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
    }
    QColor color(){
        return Qt::black; // getter is not really needed for now
    }

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QWidget *maskWidget;
};

#endif // AUTOLOADPICTURELABEL_H
