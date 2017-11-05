#include "audiodemuxer.h"

AudioDemuxer::AudioDemuxer(QObject *parent) : QObject(parent)
{
    avcodec_register_all();
    av_register_all();
    avformat_network_init();
}

AudioDemuxer::~AudioDemuxer()
{
    avformat_network_deinit();
}

void AudioDemuxer::setSourceAndLoad(QString source)
{

}
