#include "autoloadpicturelabel.h"
#include <QTimer>

AutoLoadPictureLabel::AutoLoadPictureLabel(QString url, QWidget *parent)
    :QLabel(parent)
{
    setScaledContents(true);

    network = new QNetworkAccessManager(this);
    QNetworkReply *reply = network->get(QNetworkRequest(QUrl(url)));

    connect(reply, &QNetworkReply::finished, [&, reply](){
        if (reply->error() == QNetworkReply::NoError) {
            pixmap.loadFromData(reply->readAll());
            if (!pixmap.isNull())
                setPixmap(pixmap);
        }
    });
}

AutoLoadPictureLabel::~AutoLoadPictureLabel()
{

}

BannerLabel::BannerLabel(QString url, QWidget *parent)
    : AutoLoadPictureLabel(url, parent)
{
    maskWidget = new QWidget(this);
    setColor(QColor(0, 0, 0, 160));
}

BannerLabel::~BannerLabel()
{

}

void BannerLabel::resizeEvent(QResizeEvent *event)
{
    maskWidget->resize(this->size());
    maskWidget->move(0, 0);
}
