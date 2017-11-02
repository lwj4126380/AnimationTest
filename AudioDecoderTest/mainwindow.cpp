#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>
#include <QDebug>
#include <QFile>

FILE *fp_open;
#define MAX_AUDIO_FRAME_SIZE 192000
#define ONE_PACKET_LEN  32768

int fill_iobuffer(void * ptr,uint8_t *buf, int bufsize){
    AudioThread *at = (AudioThread *)ptr;
    bool b = true;
    QByteArray qa = at->music_data.take_data(bufsize, &b);
    if (!b) {
        qDebug() << "cancel read";
        return -1;
    }
    qDebug() << qa.size();

    if (!qa.size())
        return AVERROR_EOF;
    else {
        memcpy(buf, qa.data(), qa.size());
        return qa.size();
    }
}

int64_t SeekFunc(void* ptr, int64_t pos, int whence)
{
    AudioThread *at = (AudioThread *)ptr;
    if (whence == AVSEEK_SIZE) {
        // return the file size if you wish to

        return 4602255;
    }
qDebug() << "do seek" << pos;
    at->music_data.cur_pos = pos;

    return pos;
}


void DataBuffer::put_data(QByteArray &data)
{
    QWriteLocker locker(&lock);
    music_data.append(data);
    if (music_data.size()-cur_pos >= ONE_PACKET_LEN || music_data.size() == total_size) {
        cond_empty.wakeAll();
    }
}

QByteArray DataBuffer::take_data(int64_t size, bool *b_success)
{
    QByteArray data;
    QWriteLocker locker(&lock);
    if (b_success)
        *b_success = true;

    if (music_data.size()-cur_pos >= ONE_PACKET_LEN) {
        data = music_data.mid(cur_pos, size);
        cur_pos += size;
    } else {
        if (music_data.size() == total_size) {
            int64_t len = music_data.size()-cur_pos;
            data = music_data.mid(cur_pos);
            cur_pos += len;
        } else {
            cond_empty.wait(&lock);
            if (cancel_read) {
                if (b_success)
                    *b_success = false;
            } else {
                int64_t act_len = music_data.size()-cur_pos >= ONE_PACKET_LEN ? ONE_PACKET_LEN : music_data.size()-cur_pos;
                data = music_data.mid(cur_pos, act_len);
                cur_pos += act_len;
            }
        }
    }
    return data;
}

AudioThread::AudioThread(QObject *parent) : QThread(parent),
    audio_stream(-1),
    music_data(4602255),
    skip_read(false),
    is_seeking(false)
{

    QFile *file = new QFile("J:\\test.mp3", this);
    file->open(QFile::ReadOnly);
    read_timer.setInterval(200);
    connect(&read_timer, &QTimer::timeout, this, [=](){
        QByteArray qa = file->read(20480);
        if (qa.size())
            music_data.put_data(qa);
        if (!qa.size()) {
            qDebug() << "reach end, total size is: " << music_data.music_data.size();
            read_timer.stop();
        }
    });
    read_timer.start();
}

void AudioThread::seek(qint64 pos)
{
    class SeekTask : public QRunnable {
    public:
        SeekTask(AudioThread *dt, qint64 t)
            : audio_thread(dt)
            , position(t)
        {}
        void run() {
            audio_thread->seekInternal(position);
        }
    private:
        AudioThread *audio_thread;
        qint64 position;
    };
    if (is_seeking) {
        music_data.cancel_read = true;
    }
    newSeekRequest(new SeekTask(this, pos));
}

void AudioThread::seekInternal(qint64 pos)
{
    qDebug() << "seek start--------";

    is_seeking = true;
    music_data.cancel_read = false;
    int ret = av_seek_frame(format_context, -1, pos*AV_TIME_BASE , AVSEEK_FLAG_ANY);
    is_seeking = false;
    qDebug() << "seek result " << ret;
}

void AudioThread::newSeekRequest(QRunnable *r)
{
    if (seek_tasks.size() >= seek_tasks.capacity()) {
        QRunnable *r = seek_tasks.take();
        if (r && r->autoDelete())
            delete r;
    }
    seek_tasks.put(r);
}

void AudioThread::processNextSeekTask()
{
    if (seek_tasks.isEmpty())
        return;
    QRunnable *task = seek_tasks.take();
    if (!task)
        return;
    task->run();
    if (task->autoDelete())
        delete task;
}

void AudioThread::run()
{
    qDebug() << "BBBBBB  " << QThread::currentThreadId();
    fp_open = fopen("J:\\test.mp3", "rb");

    char b[1024] = {0};

    av_register_all();
    avcodec_register_all();

    format_context = nullptr;
    format_context = avformat_alloc_context();

    unsigned char * iobuffer=(unsigned char *)av_malloc(ONE_PACKET_LEN);
    AVIOContext *avio =avio_alloc_context(iobuffer, ONE_PACKET_LEN,0,this,fill_iobuffer,nullptr, SeekFunc);
    format_context->pb=avio;
    format_context->flags = AVFMT_FLAG_CUSTOM_IO;
    int err = avformat_open_input(&format_context, "", nullptr, nullptr);
    av_strerror(err, b, 1024);
    if (err < 0) {
        qDebug() << "avformat_open_input error";
        return;
    }

    if(avformat_find_stream_info(format_context, NULL)<0){
        printf("Couldn't find stream information.\n");
        return;
    }
    // Dump valid information onto standard error
    av_dump_format(format_context, 0, NULL, false);

    // Find the first audio stream
    for(int i=0; i < format_context->nb_streams; i++)
        if(format_context->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO){
            audio_stream=i;
            break;
        }

    if(audio_stream==-1){
        printf("Didn't find a audio stream.\n");
        return;
    }

    // Get a pointer to the codec context for the audio stream
    AVCodecContext *pCodecCtx=format_context->streams[audio_stream]->codec;

    // Find the decoder for the audio stream
    AVCodec *pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL){
        printf("Codec not found.\n");
        return;
    }

    // Open codec
    if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){
        printf("Could not open codec.\n");
        return;
    }


    FILE *pFile=NULL;
    pFile=fopen("output.pcm", "wb");

    AVPacket *packet=(AVPacket *)malloc(sizeof(AVPacket));
    av_init_packet(packet);

    //Out Audio Param
    uint64_t out_channel_layout=AV_CH_LAYOUT_STEREO;
    //AAC:1024  MP3:1152
    int out_nb_samples=pCodecCtx->frame_size;
    AVSampleFormat out_sample_fmt=AV_SAMPLE_FMT_S16;
    int out_sample_rate=44100;
    int out_channels=av_get_channel_layout_nb_channels(out_channel_layout);
    //Out Buffer Size
    int out_buffer_size=av_samples_get_buffer_size(NULL,out_channels ,out_nb_samples,out_sample_fmt, 1);

    uint8_t *out_buffer=(uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE*2);

    AVFrame *pFrame;
    pFrame=av_frame_alloc();

    printf("Bitrate:\t %3d\n", format_context->bit_rate);
    printf("Decoder Name:\t %s\n", pCodecCtx->codec->long_name);
    printf("Channels:\t %d\n", pCodecCtx->channels);
    printf("Sample per Second\t %d \n", pCodecCtx->sample_rate);

    uint32_t ret,len = 0;
    int got_picture;
    int index = 0;
    //FIX:Some Codec's Context Information is missing
    int64_t in_channel_layout=av_get_default_channel_layout(pCodecCtx->channels);
    //Swr
    struct SwrContext *au_convert_ctx;
    au_convert_ctx = swr_alloc();
    au_convert_ctx=swr_alloc_set_opts(au_convert_ctx,out_channel_layout, out_sample_fmt, out_sample_rate,
                                      in_channel_layout,pCodecCtx->sample_fmt , pCodecCtx->sample_rate,0, NULL);
    swr_init(au_convert_ctx);

//    seek(20);

    while (true) {
//                QThread::msleep(1000);
        processNextSeekTask();
        qDebug() << music_data.cancel_read;

        int ret = av_read_frame(format_context, packet);
        if (ret < 0) {
            qDebug() << "av_read_frame error, code is: " << ret;
            av_strerror(ret, b, 1024);
            qDebug() << b;
            continue;
        }
        if(packet->stream_index==audio_stream){

            ret = avcodec_decode_audio4( pCodecCtx, pFrame,&got_picture, packet);
            if ( ret < 0 ) {
                printf("Error in decoding audio frame.\n");
                return;
            }
            if ( got_picture > 0 ){
                swr_convert(au_convert_ctx,&out_buffer, MAX_AUDIO_FRAME_SIZE,(const uint8_t **)pFrame->data , pFrame->nb_samples);

                qDebug() << QString("index:%1    pts:%2     packet size:%3").arg(QString::number(index)).arg(QString::number(packet->pts)).arg(QString::number(packet->size));

                fwrite(out_buffer, 1, out_buffer_size, pFile);

                index++;
            }
        }
        av_free_packet(packet);
    }

    swr_free(&au_convert_ctx);
}
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    at = std::make_shared<AudioThread>();
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_pushButton_clicked()
{
    at->start();
}

void MainWindow::on_pushButton_2_clicked()
{
//    at->music_data.put_data(QByteArray('a', 100));
}

void MainWindow::on_pushButton_3_clicked()
{
    at->seek(ui->lineEdit->text().toInt());
}
