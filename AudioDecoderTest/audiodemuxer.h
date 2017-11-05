#ifndef AUDIODEMUXER_H
#define AUDIODEMUXER_H

#include <QObject>
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
}


class AudioDemuxer : public QObject
{
    Q_OBJECT
public:
    explicit AudioDemuxer(QObject *parent = 0);
    ~AudioDemuxer();

    void setSourceAndLoad(QString source);

signals:

public slots:

private:
    AVFormatContext *format_ctx;
};

#endif // AUDIODEMUXER_H
