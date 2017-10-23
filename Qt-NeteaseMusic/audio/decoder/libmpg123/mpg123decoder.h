#ifndef MPG123DECODER_H
#define MPG123DECODER_H

#include <QObject>
#include <mpg123.h>
#include "httpprovider.h"

class Mpg123Decoder : public QObject {
    Q_OBJECT
public:
    Mpg123Decoder();
    virtual ~Mpg123Decoder();

    bool Open(HttpProvider *dataProvider);
    double SetPosition(double seconds);
    bool GetBuffer();
    void Destroy();

private:
    bool Feed();

private:
    mpg123_handle *decoder;
    HttpProvider *dataProvider;

    unsigned long cachedLength;
    long sampleRate;
    int channels;
    int sampleSizeBytes;
    int lastMpg123Status;
};

#endif // MPG123DECODER_H
