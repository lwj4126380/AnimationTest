#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H

#include <QThread>
#include "PacketBuffer.h"
#include "AVCompat.h"

#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <QtCore/QSemaphore>
#include <QtCore/QVariant>
#include <QtCore/QWaitCondition>
#include <QRunnable>
#include <QQueue>
#if QT_VERSION >= QT_VERSION_CHECK(4, 7, 0)
#include <QtCore/QElapsedTimer>
#else
#include <QtCore/QTime>
typedef QTime QElapsedTimer;
#endif

class AudioThread : public QThread
{
    Q_OBJECT
public:
    AudioThread(QObject *parent = Q_NULLPTR);
    ~AudioThread();
    bool isPaused() const;

protected:
    void run();
    void resetState();
    bool processNextTask();
    bool tryPause(unsigned long timeout = 100);
    bool decode(const Packet &packet);

public slots:
    void pause(bool p);

signals:
    void eofDecoded();
    void seekFinished(qint64 timestamp);

private:
    BlockingQueue<QRunnable*> tasks;
    bool seek_requested;
    PacketBuffer packets;
    bool paused, next_pause;
    volatile bool stop;
    QSemaphore sem;
    QWaitCondition cond; //pause
    qreal render_pts0;
    QMutex mutex;


    AVCodecContext *codec_ctx;
    QByteArray decoded;
    AVFrame *frame;
    int undecoded_size;
};

#endif // AUDIOTHREAD_H
