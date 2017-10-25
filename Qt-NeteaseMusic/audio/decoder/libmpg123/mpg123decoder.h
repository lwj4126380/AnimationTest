#ifndef MPG123DECODER_H
#define MPG123DECODER_H

#include <QThread>
#include <mpg123.h>
#include "httpprovider.h"

class Mpg123Decoder : public QThread {
    Q_OBJECT
public:
    Mpg123Decoder(QObject *parent = Q_NULLPTR);
    virtual ~Mpg123Decoder();

    bool Open(HttpProvider *dataProvider);
    double SetPosition(double seconds);
    void Destroy();

protected:
    void run();

private:
    bool Feed();

private:
    mpg123_handle *decoder;
    HttpProvider *dataProvider;
    bool bEnd;

    unsigned long cachedLength;
    long sampleRate;
    int channels;
    int sampleSizeBytes;
    int lastMpg123Status;
};

#endif // MPG123DECODER_H
