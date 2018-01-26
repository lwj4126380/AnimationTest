#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <memory>
#include <QTimer>
#include <QQueue>
#include <QRunnable>
#include <QMutex>
#include "portaudio.h"
#include "BlockingQueue.h"
#include "RawAudioBuffer.h"
#include <PacketBuffer.h>
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
}

namespace Ui {
class MainWindow;
}

class DemuxThread : public QThread
{
    Q_OBJECT
public:
    explicit DemuxThread(QObject *parent = Q_NULLPTR);
    void seek(qint64 pos); //ms

protected:
    virtual void run();

private:
    void seekInternal(qint64 pos);
    void newSeekRequest(QRunnable *r);
    void processNextSeekTask();

private:
    BlockingQueue<QRunnable*> seek_tasks;
    AVFormatContext *format_context;
    int              audio_stream;
    QTimer           read_timer;
    RawAudioBuffer   music_data;
    PacketBuffer packets;
    bool             skip_read;

    QMutex mutex;

    friend class MainWindow;

    friend int64_t SeekFunc(void* ptr, int64_t pos, int whence);
    friend int fill_iobuffer(void * opaque,uint8_t *buf, int bufsize);

};
class QFile;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool initAudio();
    QFile *fff;

    static int PACallback( const void *inputBuffer, void *outputBuffer,
                                unsigned long len,
                                const PaStreamCallbackTimeInfo* timeInfo,
                                PaStreamCallbackFlags statusFlags,
                                void *opaque );

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();
private:
    Ui::MainWindow *ui;
    std::shared_ptr<DemuxThread> at;
};

#endif // MAINWINDOW_H
