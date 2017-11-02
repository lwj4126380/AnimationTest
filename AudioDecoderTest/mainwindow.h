#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <memory>
#include <QTimer>
#include <QQueue>
#include <QRunnable>
#include "BlockingQueue.h"
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
}

namespace Ui {
class MainWindow;
}

class DataBuffer {

public:
    DataBuffer(int64_t size):total_size(size), cur_pos(0), cancel_read(false) {}
    void put_data(QByteArray &data);
    QByteArray take_data(int64_t size, bool *b_success);

private:
    QByteArray music_data;
    int64_t    cur_pos;
    mutable QReadWriteLock lock;
    QWaitCondition cond_empty;
    QMutex     mutex;
    int64_t    total_size;
    bool       cancel_read;

    friend class AudioThread;
    friend int64_t SeekFunc(void* ptr, int64_t pos, int whence);
    friend int fill_iobuffer(void * opaque,uint8_t *buf, int bufsize);

};

class AudioThread : public QThread
{
    Q_OBJECT
public:
    explicit AudioThread(QObject *parent = Q_NULLPTR);
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
    DataBuffer       music_data;
    bool             skip_read;
    bool             is_seeking;

    friend class MainWindow;

    friend int64_t SeekFunc(void* ptr, int64_t pos, int whence);
    friend int fill_iobuffer(void * opaque,uint8_t *buf, int bufsize);

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<AudioThread> at;
};

#endif // MAINWINDOW_H
