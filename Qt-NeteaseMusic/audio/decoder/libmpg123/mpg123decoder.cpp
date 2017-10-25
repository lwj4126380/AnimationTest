#include "mpg123decoder.h"
#include <QDebug>

#include "Mpg123Decoder.h"
#include <stdio.h>
#include <QTimer>
#define STREAM_FEED_SIZE 1024 * 32

FILE *ff;

Mpg123Decoder::Mpg123Decoder(QObject *parent)
    : QThread(parent)
    , cachedLength(0)
    , decoder(NULL)
    , sampleRate(44100)
    , channels(2)
    , lastMpg123Status(MPG123_NEED_MORE)
    , bEnd(false){
    mpg123_init();
    this->decoder = mpg123_new(NULL, NULL);
    this->sampleSizeBytes = sizeof(float);
    ff = fopen("J:\\test.pcm", "wb");
}

Mpg123Decoder::~Mpg123Decoder() {
    if (this->decoder) {
        mpg123_close(this->decoder);
        mpg123_delete(this->decoder);
        this->decoder = NULL;
    }
    fclose(ff);
}

void Mpg123Decoder::Destroy() {
    delete this;
}

void Mpg123Decoder::run()
{
    unsigned char targetBuffer[STREAM_FEED_SIZE] = {0};

    bool done = false;

    size_t bytesWritten = 0;

    while (!bEnd) {
//        QThread::sleep(5);
        int readResult = mpg123_read(
                    this->decoder,
                    targetBuffer,
                    STREAM_FEED_SIZE,
                    &bytesWritten);

        switch (readResult) {
        case MPG123_OK:
        case MPG123_DONE:
        case MPG123_ERR:
            break;

        case MPG123_NEED_MORE:
            if (!this->Feed()) {
                done = true;
            }
            break;

        case MPG123_NEW_FORMAT:
        {
            int encoding = 0;

            mpg123_getformat(
                        this->decoder,
                        &this->sampleRate,
                        &this->channels,
                        &encoding);

            qDebug() << "output format:"
                        "\n  type: " << encoding <<
                        "\n  rate " << this->sampleRate <<
                        "\n  channels " << this->channels << "\n";
        }
            break;
        }
//        if (readResult == MPG123_OK) {
            qDebug() << "GGGGG " << bytesWritten;
            fwrite(targetBuffer, 1, bytesWritten, ff);
//        }
        fflush(ff);
    }
}

double Mpg123Decoder::SetPosition(double second) {
    off_t seekToFileOffset = 0;
    off_t seekToSampleOffset = (off_t) (second * (double)this->sampleRate);
    off_t *indexOffset = 0;
    off_t indexSet = 0;
    size_t indexFill = 0;
    int err = mpg123_index(this->decoder, &indexOffset, &indexSet, &indexFill);

    off_t seekedTo = 0;
    int feedMore = 20;
    while((seekedTo = mpg123_feedseek(
               this->decoder,
               seekToSampleOffset,
               SEEK_SET,
               &seekToFileOffset)) == MPG123_NEED_MORE && feedMore > 0)
    {
        if (!this->Feed()) {
            feedMore = 0;
        }

        feedMore--;
    }

    //    if (seekedTo >= 0) {
    //        if (this->fileStream->SetPosition(seekToFileOffset)) {
    //            return second;
    //        }
    //    }

    return -1;
}

bool Mpg123Decoder::Feed() {
    if (dataProvider) {
        QByteArray data;
        dataProvider->getMoreAudioData(data);
        if (data.size()) {
//            qDebug() << "AAAAAAAAAA  " << data.size();
            if (mpg123_feed(this->decoder, (unsigned char *)data.data(), data.size()) == MPG123_OK) {
                return true;
            }
        }
    }

    return false;
}

bool Mpg123Decoder::Open(HttpProvider *dataProvider){
    if (this->decoder && dataProvider) {
        this->dataProvider = dataProvider;
        if (mpg123_open_feed(this->decoder) == MPG123_OK) {
            int result = mpg123_param(
                        this->decoder,
                        MPG123_ADD_FLAGS,
                        MPG123_FUZZY | MPG123_SEEKBUFFER | MPG123_GAPLESS | MPG123_QUIET,
                        0);

            /* reset output table */
            mpg123_format_none(this->decoder);

            /* force the output encoding to float32. note that this needs to
            be done before any data is fed to the decoder! */
            mpg123_format(
                        this->decoder,
                        this->sampleRate,
                        MPG123_STEREO,
                        MPG123_ENC_FLOAT_32);

            return true;
        }
    }

    return false;
}
