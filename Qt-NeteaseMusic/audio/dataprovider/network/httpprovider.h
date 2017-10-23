#ifndef HTTPPROVIDER_H
#define HTTPPROVIDER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class HttpProvider : public QObject
{
    Q_OBJECT
public:
    explicit HttpProvider(QString url, QObject *parent = 0);
    void getRemainData(QByteArray &data);

public slots:
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onReadyRead();
    void onNetworkFinished();

private:
    QNetworkAccessManager network;
    QMutex mutex;
    QByteArray cacheData;
};

#endif // HTTPPROVIDER_H
