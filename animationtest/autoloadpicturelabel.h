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
public:
    BannerLabel(QString url, QWidget *parent=Q_NULLPTR);
    ~BannerLabel();
    void showMask(bool b);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QWidget *maskWidget;
};

#endif // AUTOLOADPICTURELABEL_H
