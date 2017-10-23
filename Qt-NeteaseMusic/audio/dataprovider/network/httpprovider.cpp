#include "httpprovider.h"
#include <QDebug>

HttpProvider::HttpProvider(QString url, QObject *parent) : QObject(parent)
{
    QNetworkReply *reply = network.get(QNetworkRequest(QUrl(url)));
    connect(reply, &QNetworkReply::downloadProgress, this, &HttpProvider::onDownloadProgress);
    connect(reply, &QNetworkReply::readyRead, this, &HttpProvider::onReadyRead);
    connect(reply, &QNetworkReply::finished, this, &HttpProvider::onNetworkFinished);
}

void HttpProvider::getMoreAudioData(QByteArray &data)
{
    mutex.lock();
    data = cacheData;
    cacheData.clear();
    mutex.unlock();
}

void HttpProvider::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{

}

void HttpProvider::onReadyRead()
{
    QNetworkReply *reply = (QNetworkReply *)sender();
    QByteArray data = reply->readAll();
    mutex.lock();
    cacheData.append(data);
    mutex.unlock();
}

void HttpProvider::onNetworkFinished()
{
    QNetworkReply *reply = (QNetworkReply *)sender();
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "error in download music";
    } else {
        qDebug() << "finish download";
    }
}
